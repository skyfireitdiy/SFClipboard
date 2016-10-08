#include "ClipBoardContent.h"
#include <Global.h>
#include <QMimeData>
#include <QUrl>
#include <QPixmap>
#include <QFile>
#include <QDebug>
#include <QApplication>

int  TEXT =1;
int  HTML =2;
int  IMAGE =4;
int  COLOR =8;
int  URLS =16;


ClipBoardContent::ClipBoardContent(QObject *parent) :QObject(parent),
    pClip(QApplication::clipboard()),
    data_type(0),
    max_record_count(0),
    selecting(false),
    editing(false),
    enable(true)
{
    read_setting();
    connect(pClip,SIGNAL(dataChanged()),this,SLOT(on_clip_data_changed()));
}


void ClipBoardContent::on_clip_data_changed(){
    if(!enable)
        return;
    if(editing)
        return;
    if(selecting){
        selecting=false;
        return;
    }
    const QMimeData *temp_data=pClip->mimeData();
    Data tempData;
    tempData.type=0;
    if(temp_data->hasImage()&&data_type&IMAGE){
        tempData.image=temp_data->imageData();
        tempData.type|=IMAGE;
    }
    if(temp_data->hasColor()&&data_type&COLOR){
        tempData.color=temp_data->colorData();
        tempData.type|=COLOR;
    }
    if(temp_data->hasHtml()&&data_type&HTML){
        tempData.html=temp_data->html();
        tempData.type|=HTML;
    }
    if(temp_data->hasText()&&data_type&TEXT){
        tempData.text=temp_data->text();
        tempData.type|=TEXT;
    }
    if(temp_data->hasUrls()&&data_type&URLS){
        tempData.urls=temp_data->urls();
        tempData.type|=URLS;
    }
    if(tempData.type==0){
        return ;
    }
    data.push_front(tempData);
    if(max_record_count>0&&data.size()>max_record_count)
        data.pop_back();
    if(is_auto_save){
        save_to_file(tempData);
    }
    emit data_changed(data);
}

void ClipBoardContent::read_setting(){
    data_type=pSettings->value("data_type",0xff).toInt();
    max_record_count=pSettings->value("max_record_count",0).toInt();
    enable=pSettings->value("enable",true).toBool();
    is_auto_save=pSettings->value("auto_save",false).toBool();
    auto_save_file=pSettings->value("auto_save_file","").toString();
    flush_record();
}

void ClipBoardContent::flush_record(){
    while(max_record_count>0&&max_record_count<data.size())
        data.pop_back();
    emit data_changed(data);
}

void ClipBoardContent::on_set_clip_data(int index){
    if(index<0||index>data.size())
        return;
    selecting=true;
    Data t=data.at(index);
    QMimeData *mime=new QMimeData;
    if(t.type&TEXT)
        mime->setText(t.text);
    if(t.type&HTML)
        mime->setHtml(t.html);
    if(t.type&IMAGE)
        mime->setImageData(t.image);
    if(t.type&URLS)
        mime->setUrls(t.urls);
    if(t.type&COLOR)
        mime->setColorData(t.color);
    pClip->setMimeData(mime);
}

void ClipBoardContent::on_data_edit(int index){
    if(index<0||index>=data.size())
        return;
    editing=true;
    Data t=data.at(index);
    emit edit_data(t,index);
}

void ClipBoardContent::on_set_data(Data dt, int index){
    if(index>=0&&index<data.size())
        data[index]=dt;
    editing=false;
    on_set_clip_data(index);
    emit data_changed(data);
}

void ClipBoardContent::on_delete_record(int index){
    if(index<0||index>=data.size())
        return;
    data.removeAt(index);
    emit data_changed(data);
}


void ClipBoardContent::save_to_file(Data dt){
    if(auto_save_file.isEmpty())
        return;
    QSettings st(auto_save_file,QSettings::IniFormat);
    if(!st.isWritable())
        return;
    int count=st.value("count",0).toInt();
    count++;
    st.setValue("count",count);
    st.setValue("item"+QString::number(count)+"/type",dt.type);
    st.setValue("item"+QString::number(count)+"/text",dt.text);
    st.setValue("item"+QString::number(count)+"/html",dt.html);
    st.setValue("item"+QString::number(count)+"/image",dt.image);
    st.setValue("item"+QString::number(count)+"/color",dt.color);
    st.setValue("item"+QString::number(count)+"/urls/count",dt.urls.count());
    for(int i=0;i<dt.urls.count();++i)
        st.setValue("item"+QString::number(count)+"/urls/item"+QString::number(i+1),dt.urls.at(i));
}

void ClipBoardContent::on_save_to_file(QString file_name){
    if(file_name.isEmpty())
        return;
    QSettings st(file_name,QSettings::IniFormat);
    if(!st.isWritable())
        return;
    int count=st.value("count",0).toInt();
    count++;
    for(int i=data.size()-1;i>=0;--i){
        st.setValue("count",count);
        st.setValue("item"+QString::number(count)+"/type",data[i].type);
        st.setValue("item"+QString::number(count)+"/text",data[i].text);
        st.setValue("item"+QString::number(count)+"/html",data[i].html);
        st.setValue("item"+QString::number(count)+"/image",data[i].image);
        st.setValue("item"+QString::number(count)+"/color",data[i].color);
        st.setValue("item"+QString::number(count)+"/urls/count",data[i].urls.count());
        for(int i=0;i<data[i].urls.count();++i)
            st.setValue("item"+QString::number(count)+"/urls/item"+QString::number(i+1),data[i].urls.at(i));
        count++;
    }
}


void ClipBoardContent::on_load_from_file(QString file_name){
    if(file_name.isEmpty())
        return;
    QSettings st(file_name,QSettings::IniFormat);
    data.clear();
    Data temp_data;
    int num=st.value("count",0).toInt();
    for(int count=1;count<=num;++count){
        temp_data.clear();
        temp_data.type=st.value("item"+QString::number(count)+"/type",0).toInt();
        temp_data.text=st.value("item"+QString::number(count)+"/text","").toString();
        temp_data.html=st.value("item"+QString::number(count)+"/html").toString();
        temp_data.image=st.value("item"+QString::number(count)+"/image");
        temp_data.color=st.value("item"+QString::number(count)+"/color");
        int t=st.value("item"+QString::number(count)+"/urls/count").toInt();
        for(int i=0;i<t;++i){
            temp_data.urls.append(st.value("item"+QString::number(count)+"/urls/item"+QString::number(i+1)).toUrl());
        }
        data.insert(data.begin(),temp_data);
    }
    emit data_changed(data);
}


void ClipBoardContent::on_export_image(QString dir){
    if(dir.isEmpty())
        return;
    if(!dir.endsWith("/"))
        dir+="/";
    int count=0;
    for(auto p:data){
        if(p.type&IMAGE){
            QPixmap::fromImage(qvariant_cast<QImage>(p.image)).save(dir+QString::number(count)+".png");
            count++;
        }
    }
}


void ClipBoardContent::on_export_urls(QString file_name){
    QFile file(file_name);
    file.open(QFile::WriteOnly);
    for(auto p:data){
        if(p.type&URLS){
            for(auto q:p.urls){
                file.write((q.toString()+"\n").toUtf8());
            }
        }
    }
    file.close();
}


void ClipBoardContent::on_export_text(QString dir){
    if(dir.isEmpty())
        return;
    if(!dir.endsWith("/"))
        dir+="/";
    int count=0;
    for(auto p:data){
        if(p.type&TEXT){
            QFile file(dir+QString::number(count)+".txt");
            file.open(QFile::WriteOnly);
            file.write(p.text.toUtf8());
            file.close();
            count++;
        }
    }
}


void ClipBoardContent::on_export_text_single(QString file_name){
    QFile file(file_name);
    file.open(QFile::WriteOnly);
    int count=0;
    for(auto p:data){
        if(p.type&TEXT){
            count++;
            file.write((QString::number(count)+"------------------------------\n\n"+p.text+"\n\n").toUtf8());
        }
    }
    file.close();
}



void ClipBoardContent::on_export_html(QString dir){
    if(dir.isEmpty())
        return;
    if(!dir.endsWith("/"))
        dir+="/";
    int count=0;
    for(auto p:data){
        if(p.type&HTML){
            QFile file(dir+QString::number(count)+".html");
            file.open(QFile::WriteOnly);
            file.write("<meta charset=\"utf-8\" />");
            file.write(p.html.toUtf8());
            file.close();
            count++;
        }
    }
}


void ClipBoardContent::on_export_html_single(QString file_name){
    QFile file(file_name);
    file.open(QFile::WriteOnly);
    int count=0;
    file.write("<meta charset=\"utf-8\" />");
    for(auto p:data){
        if(p.type&HTML){
            count++;
            file.write((QString::number(count)+"------------------------------<br/><br/>"+p.html+"<br/><br/>").toUtf8());
        }
    }
    file.close();
}


void ClipBoardContent::on_clear_all(){
    data.clear();
    emit data_changed(data);
}

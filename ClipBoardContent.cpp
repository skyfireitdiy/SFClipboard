#include "ClipBoardContent.h"
#include <Global.h>
#include <QMimeData>
#include <QUrl>

#include <QDebug>

int  TEXT =1;
int  HTML =2;
int  IMAGE =4;
int  COLOR =8;
int  URLS =16;


ClipBoardContent::ClipBoardContent(QObject *parent) : QObject(parent),selecting(false),enable(true),editing(false)
{
    pClip=pApp->clipboard();
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
    if(data.size()>max_record_count)
        data.pop_back();
    if(is_auto_save){
        save_to_file(tempData);
    }
    emit data_changed(data);
}

void ClipBoardContent::read_setting(){
    data_type=pSettings->value("data_type",0xff).toInt();
    max_record_count=pSettings->value("max_record_count",20).toInt();
    enable=pSettings->value("enable",false).toBool();
    is_auto_save=pSettings->value("auto_save",false).toBool();
    auto_save_file=pSettings->value("auto_save_file","").toString();
    flush_record();
}

void ClipBoardContent::flush_record(){
    while(max_record_count<data.size())
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

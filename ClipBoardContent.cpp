#include "ClipBoardContent.h"
#include <Global.h>
#include <QMimeData>
#include <QUrl>
#include <QPixmap>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QDataStream>
#include <SFLanguage.h>

EXTERN_SF_LAN

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
    connect(this,SIGNAL(tray_msg(QString,QString)),parent,SLOT(on_tray_msg(QString,QString)));
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
    static Data oldData;
    if(oldData!=tempData){
        data.push_front(tempData);
        oldData=tempData;
    }
    if(max_record_count>0&&data.size()>max_record_count)
        data.pop_back();
    if(is_auto_save){
        save_to_file(tempData);
    }
    emit data_changed(data);
    //emit tray_msg(GS("NEW_CONTENT"),"");
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
    QSqlDatabase database=QSqlDatabase::addDatabase("QSQLITE",QLatin1String("SFClipboard_DB"));
    database.setDatabaseName(auto_save_file);
    if(!database.open())
        return;
    QStringList tables=database.tables();
    if(tables.indexOf("SFClipboard_Table")==-1){
        QSqlQuery query(database);
        if(query.prepare("create table SFClipboard_Table(type int,text blob,html blob,image blob,color blob,urls blob)")){
            if(!query.exec()){
                database.close();
                return;
            }
        }
    }
    QSqlQuery insert_sql(database);
    if(!insert_sql.prepare("insert into SFClipboard_Table values(?,?,?,?,?,?)")){
        database.close();
        return;
    }
    insert_sql.addBindValue(dt.type);
    insert_sql.addBindValue(dt.text.toLocal8Bit());
    insert_sql.addBindValue(dt.html.toLocal8Bit());
    insert_sql.addBindValue(qvariant2qbytearray(dt.image));
    insert_sql.addBindValue(qvariant2qbytearray(dt.color));
    insert_sql.addBindValue(encode_urls(dt.urls));
    insert_sql.exec();
    database.close();
}

void ClipBoardContent::on_save_to_file(QString file_name){
    if(file_name.isEmpty())
        return;
    for(auto p:data)
        save_to_file(p);
    emit tray_msg(GS("SAVE_TO_FILE"),GS("SUCCESS"));
}


void ClipBoardContent::on_load_from_file(QString file_name){
    if(file_name.isEmpty())
        return;
    data.clear();
    QSqlDatabase database=QSqlDatabase::addDatabase("QSQLITE",QLatin1String("SFClipboard_DB"));
    database.setDatabaseName(auto_save_file);
    if(!database.open())
        return;
    QStringList tables=database.tables();
    if(tables.indexOf("SFClipboard_Table")==-1){
        QSqlQuery query(database);
        if(query.prepare("create table SFClipboard_Table(type int,text blob,html blob,image blob,color blob,urls blob)")){
            if(!query.exec()){
                database.close();
                return;
            }
        }
    }
    Data temp_data;
    QSqlQuery select_sql(database);
    if(!select_sql.prepare("select * from SFClipboard_Table")){
        database.close();
        return;
    }
    if(!select_sql.exec()){
        database.close();
        return;
    }
    data.clear();
    while(select_sql.next()){
        temp_data.type=select_sql.value("type").toInt();
        temp_data.text=QString::fromLocal8Bit(select_sql.value("text").toByteArray());
        temp_data.html=QString::fromLocal8Bit(select_sql.value("html").toByteArray());
        temp_data.image=qbytearray2qvariant(select_sql.value("image").toByteArray());
        temp_data.color=qbytearray2qvariant(select_sql.value("color").toByteArray());
        temp_data.urls=decode_urls(select_sql.value("urls").toByteArray());
        data.push_front(temp_data);
    }
    emit data_changed(data);
    database.close();
    emit tray_msg(GS("LOAD_FROM_FILE"),GS("SUCCESS"));
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
    emit tray_msg(GS("EXPORT_IMAGE"),GS("SUCCESS"));
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
    emit tray_msg(GS("EXPORT_URL"),GS("SUCCESS"));
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
    emit tray_msg(GS("EXPORT_TEXT"),GS("SUCCESS"));
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
    emit tray_msg(GS("EXPORT_TEXT_SINGLE"),GS("SUCCESS"));
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
    emit tray_msg(GS("EXPORT_HTML"),GS("SUCCESS"));
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
    emit tray_msg(GS("EXPORT_HTML_SINGLE"),GS("SUCCESS"));
}


void ClipBoardContent::on_clear_all(){
    data.clear();
    emit data_changed(data);
    emit tray_msg(GS("CLEAR"),GS("SUCCESS"));
}


QByteArray ClipBoardContent::encode_urls(QList<QUrl> url){
    QStringList strlist;
    for(auto p:url){
        strlist.append(p.toString());
    }
    return qvariant2qbytearray(strlist);
}

QList<QUrl> ClipBoardContent::decode_urls(QByteArray data){
    QVariant var=qbytearray2qvariant(data);
    auto tmp=var.toStringList();
    QList<QUrl> ret;
    for(auto p:tmp){
        ret.append(QUrl(p));
    }
    return ret;
}


QByteArray ClipBoardContent::qvariant2qbytearray(QVariant t){
    QByteArray ret;
    QDataStream stream(&ret,QIODevice::WriteOnly);
    t.save(stream);
    return ret;
}

QVariant ClipBoardContent::qbytearray2qvariant(QByteArray b){
    QDataStream stream(b);
    return stream;
}

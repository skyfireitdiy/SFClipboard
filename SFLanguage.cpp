#include "SFLanguage.h"
#include <QLocale>
#include <QDebug>

SFLanguage::SFLanguage(QString language_file, QString default_lan, QObject *parent) : QObject(parent),file_name(language_file),default_language(default_lan)
{
    language_settings=new QSettings(file_name,QSettings::IniFormat,this);
    if(default_lan.isEmpty()){
        set_locate_lang();
    }else{

    }
}

QString SFLanguage::get_string(QString key, QString language){
    if(language.isEmpty()){
        if(language_settings->contains(default_language+"/"+key))
            return language_settings->value(default_language+"/"+key).toString();
        emit string_not_found(default_language+"/"+key);
    }else{
        if(language_settings->contains(language+"/"+key))
            return language_settings->value(language+"/"+key).toString();
        emit string_not_found(language+"/"+key);
    }
}

QStringList SFLanguage::get_lang_list(){
    return language_settings->childGroups();
}

void SFLanguage::set_lang(QString default_lang){
    QStringList lst=get_lang_list();
    if(lst.indexOf(default_lang)==-1){
        set_locate_lang(default_lang);
    }else{
        default_language=default_lang;
    }
}

void SFLanguage::set_locate_lang(QString default_lang){
    QStringList lst=get_lang_list();
    QString lang=QLocale::languageToString(QLocale::system().language());
    if(lst.indexOf(lang)==-1){
        if(lst.indexOf(default_lang)==-1){
            if(!lst.isEmpty()){
                default_language=lst.at(0);
            }else{
                default_language="";
                emit no_long_found();
            }
        }else{
            default_language=default_lang;
        }
    }else{
        default_language=lang;
    }
}



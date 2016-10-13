#include "SFLanguage.h"

SFLanguage::SFLanguage(QString language_file, QString default_lan, QObject *parent) : QObject(parent),file_name(language_file),default_language(default_lan)
{
    language_settings=new QSettings(file_name,QSettings::IniFormat,this);
}

QString SFLanguage::get_string(QString key, QString language){
    if(language.isEmpty()){
        return language_settings->value(default_language+"/"+key).toString();
    }else{
        return language_settings->value(language+"/"+key).toString();
    }
}

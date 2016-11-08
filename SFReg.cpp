#include "SFReg.h"

SFReg::SFReg(QString key_root, QSettings::Scope scope, QObject *parent) : QObject(parent)
{
    st.setPath(QSettings::NativeFormat,scope,key_root);
}

void SFReg::set_root(QString key_root, QSettings::Scope scope){
    st.setPath(QSettings::NativeFormat,scope,key_root);
}

void SFReg::set_value(QString key, QVariant value){
    st.setValue(key,value);
}

QVariant SFReg::value(QString key, QVariant default_value){
    return st.value(key,default_value);
}

bool SFReg::has_key(QString key){
    return st.contains(key);
}

void SFReg::delete_key(QString key){
    st.remove(key);
}

void SFReg::add_win32_auto_run(QString key, QString path){
    QSettings temp_st("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    temp_st.setValue(key,path);
}


void SFReg::delete_win32_auto_run(QString key){
    QSettings temp_st("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    temp_st.remove(key);
}

bool SFReg::has_win32_auto_run(QString key){
    QSettings temp_st("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    return temp_st.contains(key);
}


void SFReg::add_win32_auto_run_once(QString key, QString path){
    QSettings temp_st("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce",QSettings::NativeFormat);
    temp_st.setValue(key,path);
}


void SFReg::delete_win32_auto_run_once(QString key){
    QSettings temp_st("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce",QSettings::NativeFormat);
    temp_st.remove(key);
}

bool SFReg::has_win32_auto_run_once(QString key){
    QSettings temp_st("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce",QSettings::NativeFormat);
    return temp_st.contains(key);
}

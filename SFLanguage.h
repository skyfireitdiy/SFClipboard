#ifndef SFLANGUAGE_H
#define SFLANGUAGE_H

#include <QObject>
#include <QSettings>

class SFLanguage : public QObject
{
    Q_OBJECT
public:
    SFLanguage(QString language_file,QString default_lan="",QObject *parent = 0);
    SFLanguage(SFLanguage&&)=default;
signals:
    void no_long_found();
    void string_not_found(QString key);
public slots:

public:
    QString get_string(QString key,QString language="");
    QStringList get_lang_list();
    QString get_current_lang();
    void set_lang(QString default_lang);
    void set_locale_lang(QString default_lang="");
private:
    QString file_name;
    QString default_language;
    QSettings *language_settings;
};



//如果全局使用一个语言设置文件，并且对此文件操作较少，可使用以下宏定义
#define SF_LAN_INIT(x,y) SFLanguage *__lan_st=new SFLanguage(QString(x),QString(y));
#define EXTERN_SF_LAN extern SFLanguage *__lan_st;
#define GS(x) (__lan_st->get_string(QString(x)))
#define GLS(x,y) (__lan_st->get_string(QString(x),QString(y)))
#define LAN_LIST (__lan_st->get_lang_list())
#define SET_DEFAULT_LANG(x) (__lan_st->set_locale_lang(x))
#define SET_LANG(x) (__lan_st->set_lang(x))
#define GET_CURR_LANG (__lan_st->get_current_lang())


#endif // SFLANGUAGE_H

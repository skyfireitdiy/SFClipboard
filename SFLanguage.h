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

public slots:

public:
    QString get_string(QString key,QString language="");
private:
    QString file_name;
    QString default_language;
    QSettings *language_settings;
};




#define SF_LAN_INIT(x,y) SFLanguage *__lan_st=new SFLanguage(QString(#x),QString(#y));
#define EXTERN_SF_LAN extern SFLanguage *__lan_st;
#define GS(x) __lan_st->get_string(QString(#x))
#define GLS(x,y) __lan_st->get_string(QString(#x),QString(#y))



#endif // SFLANGUAGE_H

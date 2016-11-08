#ifndef SFREG_H
#define SFREG_H

#include <QObject>
#include <QSettings>
#include <QVariant>

class SFReg : public QObject
{
    Q_OBJECT
public:
    explicit SFReg(QString key_root="",QSettings::Scope scope=QSettings::SystemScope,QObject *parent = 0);
    void set_root(QString key_root,QSettings::Scope scope=QSettings::SystemScope);
    void set_value(QString key,QVariant value);
    QVariant value(QString key,QVariant default_value=QVariant());
    bool has_key(QString key);
    void delete_key(QString key);



    static void add_win32_auto_run(QString key,QString path);
    static void delete_win32_auto_run(QString key);
    static bool has_win32_auto_run(QString key);

    static void add_win32_auto_run_once(QString key,QString path);
    static void delete_win32_auto_run_once(QString key);
    static bool has_win32_auto_run_once(QString key);
signals:

public slots:

private:
    QSettings st;
};

#endif // SFREG_H

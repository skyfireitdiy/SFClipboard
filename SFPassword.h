#ifndef SFPASSWORD_H
#define SFPASSWORD_H

#include <QObject>
#include <SFAES.h>

class SFPassword : public QObject
{
    Q_OBJECT
public:
    explicit SFPassword(QByteArray ps,QObject *parent = 0);
    SFPassword(const SFPassword&)=delete;
    SFPassword(SFPassword &&other);
    operator QByteArray()const;
    const SFPassword operator=(const SFPassword&)=delete;
    QByteArray make_ps(int len,QByteArray char_set="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-=`~!@#$%^&*()_+|}{[]\\\";:'?/>.<,")const;
    static void distroy_password(QByteArray &ps);
signals:

public slots:

private:
    QByteArray ps_buffer;
    int ps_len{0};
    SFAES aes{make_ps(128),this};
};

#endif // SFPASSWORD_H

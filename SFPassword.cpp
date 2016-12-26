#include "SFPassword.h"
#include <QTime>


SFPassword::SFPassword(QByteArray ps, QObject *parent) : QObject(parent)
{
    ps_len=ps.size();
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    ps_buffer=aes.Encrypt(ps);
}

QByteArray SFPassword::make_ps(int len, QByteArray char_set) const{
    int set_len=char_set.size();
    QByteArray arr;
    for(int i=0;i<len;++i)
        arr.push_back(char_set[rand()%set_len]);
    return arr;
}

SFPassword::operator QByteArray()const{
    return aes.Decrypt(ps_buffer,ps_len);
}

SFPassword::SFPassword(SFPassword &&other){
    if(this==&other)
        return;
    memmove(this,&other,sizeof(SFPassword));
    memset(&other,0,sizeof(SFPassword));
}

void SFPassword::distroy_password(QByteArray &ps){
    memset(ps.data(),0,ps.size());
}

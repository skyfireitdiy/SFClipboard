#ifndef LINUXHOOK_H
#define LINUXHOOK_H

#include <QObject>
#include <QProcess>

class LinuxKeyHook :public QObject{
    Q_OBJECT
signals:
    void hot_copy(int num);
    void hot_delete(int num);
    void clear();
    void exit_hook();
    void need_root();
    void in_out();
public:
    LinuxKeyHook(QObject *parent=0);
    void start();
};

void unHook();
void setHook();
void reportkey(uint16_t keycode, int32_t value) ;

#endif // LINUXHOOK_H

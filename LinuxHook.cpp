#ifdef __unix
#include <LinuxHook.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <mutex>
#include <MainWidget.h>
#include <QProcess>
#include <QEventLoop>
#include <QRegExp>
#include <thread>
#include <linux/input.h>
#include <QDebug>
#include <QEventLoop>
#include <QMessageBox>
#include <SFLanguage.h>

EXTERN_SF_LAN

extern MainWidget *pMainWidget;
static bool finished=false;
static std::mutex key_mu;
QString __file_name;




void unHook(){
    key_mu.lock();
    finished=true;
    key_mu.unlock();
}

void setHook(){
    std::thread([&](){
        LinuxKeyHook hook;
        QObject::connect(&hook,SIGNAL(hot_copy(int)),pMainWidget,SLOT(on_hot_copy(int)));
        QObject::connect(&hook,SIGNAL(hot_delete(int)),pMainWidget,SLOT(on_hot_delete(int)));
        QObject::connect(&hook,SIGNAL(clear()),pMainWidget,SLOT(on_clear_all()));
        QObject::connect(&hook,SIGNAL(need_root()),pMainWidget,SLOT(on_need_root)));
        QEventLoop event;
        QObject::connect(&hook,SIGNAL(exit_hook()),&event,SLOT(quit()));
        hook.start();
        event.exec(0);
    }).detach();
}



QString run_process(QString cmd){
    QProcess pro;
    QEventLoop event;
    QString ret_str;
    QObject::connect(&pro,SIGNAL(finished(int)),&event,SLOT(quit()));
    QObject::connect(&pro,&QProcess::readyReadStandardOutput,[&](){ret_str+=pro.readAllStandardOutput();});
    pro.start(cmd);
    event.exec();
    return ret_str;
}

LinuxKeyHook::LinuxKeyHook(QObject *parent):
    QObject(parent)
{
    QString input_dev_content=run_process("cat /proc/bus/input/devices");
    int index=input_dev_content.indexOf("keyboard");
    if(index==-1){
        return;
    }
    QRegExp re("event[0-9]+");
    if(input_dev_content.indexOf(re,index)==-1){
        return;
    }
    __file_name="/dev/input/"+re.cap(0);
}

void LinuxKeyHook::start(){
    int keys_fd;
    struct input_event t;
    if(__file_name.isEmpty()){
        emit exit_hook();
        return;
    }
    keys_fd = open (__file_name.toLocal8Bit(), O_RDONLY);
    if (keys_fd <= 0){
        emit exit_hook();
        emit need_root();
        return;
    }
    bool ctrl=false;
    bool alt=false;
    while (1)  {
        key_mu.lock();
        if(finished){
            key_mu.unlock();
            close(keys_fd);
            emit exit_hook();
            return;
        }
        key_mu.unlock();
        if (read(keys_fd, &t, sizeof (t)) == sizeof (t))  {
            if (t.type == EV_KEY){
                if (t.value == 1){
                    if(t.code==KEY_LEFTCTRL){
                        ctrl=true;
                    }else if(t.code==KEY_LEFTALT){
                        alt=true;
                    }else if(t.code==KEY_KP0){
                        if(ctrl)
                            emit hot_copy(0);
                        else if(alt)
                            emit hot_delete(0);
                    }else if(t.code==KEY_KP1){
                        if(ctrl)
                            emit hot_copy(1);
                        else if(alt)
                            emit hot_delete(1);
                    }else if(t.code==KEY_KP2){
                        if(ctrl)
                            emit hot_copy(2);
                        else if(alt)
                            emit hot_delete(2);
                    }else if(t.code==KEY_KP3){
                        if(ctrl)
                            emit hot_copy(3);
                        else if(alt)
                            emit hot_delete(3);
                    }else if(t.code==KEY_KP4){
                        if(ctrl)
                            emit hot_copy(4);
                        else if(alt)
                            emit hot_delete(4);
                    }else if(t.code==KEY_KP5){
                        if(ctrl)
                            emit hot_copy(5);
                        else if(alt)
                            emit hot_delete(5);
                    }else if(t.code==KEY_KP6){
                        if(ctrl)
                            emit hot_copy(6);
                        else if(alt)
                            emit hot_delete(6);
                    }else if(t.code==KEY_KP7){
                        if(ctrl)
                            emit hot_copy(7);
                        else if(alt)
                            emit hot_delete(7);
                    }else if(t.code==KEY_KP8){
                        if(ctrl)
                            emit hot_copy(8);
                        else if(alt)
                            emit hot_delete(8);
                    }else if(t.code==KEY_KP9){
                        if(ctrl)
                            emit hot_copy(9);
                        else if(alt)
                            emit hot_delete(9);
                    }else if(t.code==KEY_C){
                        if(alt)
                            emit clear();
                    }
                }else if(t.value ==0){
                    if(t.code==KEY_LEFTCTRL){
                        ctrl=false;
                    }else if(t.code==KEY_LEFTALT){
                        alt=false;
                    }
                }
            }
        }
    }
}


void reportkey(uint16_t keycode, int32_t value)
{
    int fd=open(__file_name.toLocal8Bit(),O_WRONLY);
    if(fd<0){
        return;
    }
    struct input_event event;
    event.type=EV_KEY;
    event.code=keycode;
    event.value=value;
    gettimeofday(&event.time,0);
    write(fd, &event, sizeof(struct input_event));
    close(fd);
}


#endif

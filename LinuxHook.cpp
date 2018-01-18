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
#include <Global.h>

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
        QObject::connect(&hook,SIGNAL(need_root()),pMainWidget,SLOT(on_need_root()));
        QObject::connect(&hook,SIGNAL(in_out()),pMainWidget,SIGNAL(frame_in_out_sgn()));
        QObject::connect(&hook,SIGNAL(fix_float()),pMainWidget->hide_btn,SLOT(click()));
        QEventLoop event;
        QObject::connect(&hook,SIGNAL(exit_hook()),&event,SLOT(quit()));
        hook.start();
        event.exec(0);
    }).detach();
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
                    }else if(t.code==KEY_KP0 || t.code == KEY_0){
                        if(ctrl)
                            emit hot_copy(0);
                        else if(alt)
                            emit hot_delete(0);
                    }else if(t.code==KEY_KP1 || t.code == KEY_1){
                        if(ctrl)
                            emit hot_copy(1);
                        else if(alt)
                            emit hot_delete(1);
                    }else if(t.code==KEY_KP2 || t.code == KEY_2){
                        if(ctrl)
                            emit hot_copy(2);
                        else if(alt)
                            emit hot_delete(2);
                    }else if(t.code==KEY_KP3 || t.code == KEY_3){
                        if(ctrl)
                            emit hot_copy(3);
                        else if(alt)
                            emit hot_delete(3);
                    }else if(t.code==KEY_KP4 || t.code == KEY_4){
                        if(ctrl)
                            emit hot_copy(4);
                        else if(alt)
                            emit hot_delete(4);
                    }else if(t.code==KEY_KP5 || t.code == KEY_5){
                        if(ctrl)
                            emit hot_copy(5);
                        else if(alt)
                            emit hot_delete(5);
                    }else if(t.code==KEY_KP6 || t.code == KEY_6){
                        if(ctrl)
                            emit hot_copy(6);
                        else if(alt)
                            emit hot_delete(6);
                    }else if(t.code==KEY_KP7 || t.code == KEY_7){
                        if(ctrl)
                            emit hot_copy(7);
                        else if(alt)
                            emit hot_delete(7);
                    }else if(t.code==KEY_KP8 || t.code == KEY_8){
                        if(ctrl)
                            emit hot_copy(8);
                        else if(alt)
                            emit hot_delete(8);
                    }else if(t.code==KEY_KP9 || t.code == KEY_9){
                        if(ctrl)
                            emit hot_copy(9);
                        else if(alt)
                            emit hot_delete(9);
                    }else if(t.code==KEY_C){
                        if(alt)
                            emit clear();
                    }else if(t.code==KEY_S){
                        if(alt)
                            emit in_out();
                    }else if(t.code==KEY_F){
                        if(alt)
                            emit fix_float();
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

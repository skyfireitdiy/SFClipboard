#include "AutoHide.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>
#include <SFLanguage.h>

static const int hide_speed=10;
static const int show_speed=30;

EXTERN_SF_LAN

AutoHide::AutoHide(QWidget *parent) : QObject(parent),parent_widget(parent),mouse_press(false)
{
    hide_timer=new QTimer(this);
    show_timer=new QTimer(this);
    connect(hide_timer,SIGNAL(timeout()),this,SLOT(on_hide_widget()));
    connect(show_timer,SIGNAL(timeout()),this,SLOT(on_show_widget()));
    connect(this,SIGNAL(tray_msg(QString,QString)),parent,SLOT(on_tray_msg(QString,QString)));
    parent_widget->setMouseTracking(true);
    parent_widget->installEventFilter(this);
    parent_widget->setWindowFlags(parent_widget->windowFlags()|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::X11BypassWindowManagerHint|Qt::Tool);
    //emit tray_msg(GS("FLOAT"),GS("START_FLOAT"));
}


void AutoHide::on_check_if_hide(){
    QRect rect=parent_widget->geometry();
    QPoint pos=QCursor::pos();
    if(rect.contains(pos)){
        if(!show_timer->isActive()){
            show_timer->start(5);
        }
        if(hide_timer->isActive()){
            hide_timer->stop();
        }
    }else{
        if(!hide_timer->isActive()){
            hide_timer->start(10);
        }
        if(show_timer->isActive()){
            show_timer->stop();
        }
    }
}


void AutoHide::on_hide_widget(){
    QRect desktop_rect=QApplication::desktop()->rect();
    QRect widget_rect=parent_widget->geometry();
    if(desktop_rect.left()>=widget_rect.left()){
        if(desktop_rect.left()<widget_rect.right()-hide_speed){
            parent_widget->move(parent_widget->x()-hide_speed,parent_widget->y());
        }else{
            hide_timer->stop();
            parent_widget->move(-parent_widget->width()+hide_speed/2,parent_widget->y());
        }
        return;
    }
    if(desktop_rect.right()<=widget_rect.right()){
        if(desktop_rect.right()>widget_rect.left()+hide_speed){
            parent_widget->move(parent_widget->x()+hide_speed,parent_widget->y());
        }else{
            hide_timer->stop();
            parent_widget->move(desktop_rect.right()-hide_speed/2,parent_widget->y());
        }
        return;
    }
    if(desktop_rect.top()>=widget_rect.top()){
        if(desktop_rect.top()<widget_rect.bottom()-hide_speed){
            parent_widget->move(parent_widget->x(),parent_widget->y()-hide_speed);

        }else{
            hide_timer->stop();
            parent_widget->move(parent_widget->x(),-parent_widget->height()+hide_speed/2);
        }
        return;
    }
    if(desktop_rect.bottom()<=widget_rect.bottom()){
        if(desktop_rect.bottom()>widget_rect.top()+hide_speed){
            parent_widget->move(parent_widget->x(),parent_widget->y()+hide_speed);
        }else{
            hide_timer->stop();
            parent_widget->move(parent_widget->x(),desktop_rect.bottom()-hide_speed/2);
        }
        return;
    }
    hide_timer->stop();
}


void AutoHide::on_show_widget(){
    QRect desktop_rect=QApplication::desktop()->rect();
    QRect widget_rect=parent_widget->geometry();
    if(desktop_rect.left()>widget_rect.left()+show_speed){
        parent_widget->move(parent_widget->x()+show_speed,parent_widget->y());
    }else if(desktop_rect.right()<widget_rect.right()-show_speed){
        parent_widget->move(parent_widget->x()-show_speed,parent_widget->y());
    }else if(desktop_rect.top()>widget_rect.top()+show_speed){
        parent_widget->move(parent_widget->x(),parent_widget->y()+show_speed);
    }else if(desktop_rect.bottom()<widget_rect.bottom()-show_speed){
        parent_widget->move(parent_widget->x(),parent_widget->y()-show_speed);
    }else{
        show_timer->stop();
    }
}

bool AutoHide::eventFilter(QObject *watched, QEvent *event){
    if(watched==(QObject*)parent_widget){
        if(event->type()==QEvent::Enter||event->type()==QEvent::Leave){
            on_check_if_hide();
        }else if(event->type()==QEvent::MouseButtonPress){
            QMouseEvent *m=(QMouseEvent*)event;
            if(m->button()==Qt::LeftButton){
                old_pos=QCursor::pos();
                mouse_press=true;
            }
        }else if(event->type()==QEvent::MouseMove){
            if(mouse_press){
                if(QApplication::mouseButtons()&Qt::LeftButton){
                    QPoint new_pos=QCursor::pos();
                    parent_widget->move(parent_widget->pos()+new_pos-old_pos);
                    old_pos=new_pos;
                }else{
                    mouse_press=false;
                }
            }
        }else if(event->type()==QEvent::MouseButtonRelease){
            QMouseEvent *m=(QMouseEvent*)event;
            if(m->button()==Qt::LeftButton){
                mouse_press=false;
                emit pos_changed(old_pos);
            }
        }
    }
    return QObject::eventFilter(watched,event);
}


void AutoHide::on_in_out(){
    static bool in=false;
    if(in){
        in=false;
        hide_timer->start(10);
    }else{
        in=true;
        show_timer->start(5);
    }
}

AutoHide::~AutoHide(){
    //emit tray_msg(GS("FLOAT"),GS("START_FIXED"));
}

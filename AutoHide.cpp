#include "AutoHide.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>

AutoHide::AutoHide(QWidget *parent) : QObject(parent),parent_widget(parent),mouse_press(false)
{
    hide_timer=new QTimer(this);
    show_timer=new QTimer(this);
    connect(hide_timer,SIGNAL(timeout()),this,SLOT(on_hide_widget()));
    connect(show_timer,SIGNAL(timeout()),this,SLOT(on_show_widget()));
    parent_widget->setMouseTracking(true);
    parent_widget->installEventFilter(this);
    parent_widget->setWindowFlags(parent_widget->windowFlags()|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
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
        if(desktop_rect.left()<widget_rect.right()-10){
            parent_widget->move(parent_widget->x()-10,parent_widget->y());
        }else{
            hide_timer->stop();
            parent_widget->move(-parent_widget->width()+5,parent_widget->y());
        }
        return;
    }
    if(desktop_rect.right()<=widget_rect.right()){
        if(desktop_rect.right()>widget_rect.left()+10){
            parent_widget->move(parent_widget->x()+10,parent_widget->y());
        }else{
            hide_timer->stop();
            parent_widget->move(desktop_rect.right()-5,parent_widget->y());
        }
        return;
    }
    if(desktop_rect.top()>=widget_rect.top()){
        if(desktop_rect.top()<widget_rect.bottom()-10){
            parent_widget->move(parent_widget->x(),parent_widget->y()-10);

        }else{
            hide_timer->stop();
            parent_widget->move(parent_widget->x(),-parent_widget->height()+5);
        }
        return;
    }
    if(desktop_rect.bottom()<=widget_rect.bottom()){
        if(desktop_rect.bottom()>widget_rect.top()+10){
            parent_widget->move(parent_widget->x(),parent_widget->y()+10);
        }else{
            hide_timer->stop();
            parent_widget->move(parent_widget->x(),desktop_rect.bottom()-5);
        }
        return;
    }
    hide_timer->stop();
}


void AutoHide::on_show_widget(){
    QRect desktop_rect=QApplication::desktop()->rect();
    QRect widget_rect=parent_widget->geometry();
    if(desktop_rect.left()>widget_rect.left()+10){
        parent_widget->move(parent_widget->x()+10,parent_widget->y());
    }else if(desktop_rect.right()<widget_rect.right()-10){
        parent_widget->move(parent_widget->x()-10,parent_widget->y());
    }else if(desktop_rect.top()>widget_rect.top()+10){
        parent_widget->move(parent_widget->x(),parent_widget->y()+10);
    }else if(desktop_rect.bottom()<widget_rect.bottom()-10){
        parent_widget->move(parent_widget->x(),parent_widget->y()-10);
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
                qDebug()<<"Left Button Down";
            }
        }else if(event->type()==QEvent::MouseMove){
            QMouseEvent *m=(QMouseEvent*)event;
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
            }
        }
    }
    return QObject::eventFilter(watched,event);
}

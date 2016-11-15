#ifndef AUTOHIDE_H
#define AUTOHIDE_H

#include <QObject>
#include <QWidget>
#include <QTimer>

class AutoHide : public QObject
{
    Q_OBJECT
public:
    explicit AutoHide(QWidget *parent = 0);

signals:

public slots:
    void on_check_if_hide();
    void on_show_widget();
    void on_hide_widget();
    void on_in_out();
signals:
    void pos_changed(QPoint);
private:
    QWidget *parent_widget;
    QTimer *start_hide_timer;
    QTimer *hide_timer;
    QTimer *show_timer;

    QPoint old_pos;
    bool mouse_press;

    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // AUTOHIDE_H

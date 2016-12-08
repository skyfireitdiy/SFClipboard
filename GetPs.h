#ifndef GETPS_H
#define GETPS_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <SFPassword.h>


class GetPs : public QDialog
{
    Q_OBJECT
public:
    GetPs(QWidget *parent=nullptr);
    SFPassword get_ps();


private slots:
    void make_sure();
private:

    QLabel *lab;
    QLineEdit *ps;
    QPushButton *btn;

};

#endif // GETPS_H

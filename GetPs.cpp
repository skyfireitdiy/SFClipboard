#include "GetPs.h"
#include <SFLanguage.h>
#include <QHBoxLayout>

EXTERN_SF_LAN

GetPs::GetPs(QWidget *parent):QDialog(parent)
{
    lab=new QLabel(GS("ROOT_PASSWORD"),this);
    ps=new QLineEdit(this);
    ps->setEchoMode(QLineEdit::Password);
    btn=new QPushButton(GS("SURE"),this);
    QHBoxLayout *layout=new QHBoxLayout;
    setLayout(layout);
    layout->addWidget(lab);
    layout->addWidget(ps);
    layout->addWidget(btn);
    connect(btn,SIGNAL(clicked(bool)),this,SLOT(make_sure()));
    connect(ps,SIGNAL(returnPressed()),this,SLOT(make_sure()));
}


SFPassword GetPs::get_ps(){
    return SFPassword(ps->text().toLocal8Bit(),this);
}

void GetPs::make_sure(){
    done(0);
}

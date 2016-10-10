#include "TypeFilter.h"
#include <QGridLayout>
#include <Global.h>

TypeFilter::TypeFilter(QWidget *parent):QDialog(parent)
{
    setWindowTitle("类型筛选");
    QGridLayout *layout=new QGridLayout(this);
    setLayout(layout);
    enable_text=new QCheckBox("文本",this);
    enable_html=new QCheckBox("HTML",this);
    enable_url=new QCheckBox("URL",this);
    enable_image=new QCheckBox("图片",this);
    enable_color=new QCheckBox("颜色",this);
    ok_btn=new QPushButton("确定",this);
    cancle_btn=new QPushButton("取消",this);

    connect(ok_btn,SIGNAL(clicked(bool)),this,SLOT(on_ok_btn_clicked()));
    connect(cancle_btn,SIGNAL(clicked(bool)),this,SLOT(on_cancle_btn_clicked()));


    layout->addWidget(enable_text,0,0,1,2);
    layout->addWidget(enable_html,0,2,1,2);
    layout->addWidget(enable_url,0,4,1,2);
    layout->addWidget(enable_image,1,0,1,2);
    layout->addWidget(enable_color,1,2,1,2);
    layout->addWidget(ok_btn,2,1,1,2);
    layout->addWidget(cancle_btn,2,3,1,2);

    int data_type=pSettings->value("data_type",0xff).toInt();
    enable_text->setChecked(data_type&TEXT);
    enable_html->setChecked(data_type&HTML);
    enable_url->setChecked(data_type&URLS);
    enable_image->setChecked(data_type&IMAGE);
    enable_color->setChecked(data_type&COLOR);

    setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::X11BypassWindowManagerHint);
    setContentsMargins(50,20,50,20);
    setStyleSheet("*{font-weight:bold;color:white;font-family:'微软雅黑';}"
                  ".QLineEdit{background-color:#FFFFFF;color:000000;}"
                  ".QPushButton{border-image:url(:/pic/resource/btn.png);border-width:5px;}"
                  ".QPushButton:hover{border-image:url(:/pic/resource/btn_hover.png);border-width:5px;}"
                  ".QPushButton:disabled{border-image:url(:/pic/resource/btn_pressed.png);border-width:5px;}"
                  );

}

void TypeFilter::on_ok_btn_clicked(){
    done(1);
}

void TypeFilter::on_cancle_btn_clicked(){
    done(0);
}

void TypeFilter::resizeEvent(QResizeEvent *){
    QPalette pal;
    pal.setBrush(QPalette::Window,QBrush(QPixmap(":/pic/resource/background.png").scaled(size())));
    setPalette(pal);
}

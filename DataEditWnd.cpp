#include "DataEditWnd.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <Global.h>
#include <QDebug>



DataEditWnd::DataEditWnd(Data data_t, QWidget *parent):QDialog(parent),data(data_t),color_changed(false)
{
    setWindowTitle("编辑数据");
    text_group=new QGroupBox("文本",this);
    text_edit=new QPlainTextEdit(data.text,this);
    html_group=new QGroupBox("HTML",this);
    html_edit_group=new QGroupBox("编辑",this);
    html_show_group=new QGroupBox("预览",this);
    html_edit=new QPlainTextEdit(data.html,this);
    html_show=new QTextBrowser(this);
    html_show->setHtml(data.html);
    html_show->setReadOnly(true);
    image_group=new QGroupBox("图片",this);
    image_show=new QLabel(this);
    QPixmap pixmap=QPixmap::fromImage(qvariant_cast<QImage>(data.image));
    if(!pixmap.isNull())
        image_show->setPixmap(pixmap.scaled(600,400,Qt::KeepAspectRatio));
    image_path=new QLineEdit(this);
    image_path->setReadOnly(true);
    image_path_tip=new QLabel("路径",this);
    image_path_tip->setAlignment(Qt::AlignCenter);
    get_image_path_btn=new QPushButton("浏览",this);
    image_clear_btn=new QPushButton("清空",this);
    save_current_image=new QPushButton("另存图片",this);
    urls_group=new QGroupBox("URL",this);
    urls_list=new QListView(this);
    urls_model=new QStandardItemModel(this);
    for(auto p:data.urls){
        urls_model->appendRow(new QStandardItem(p.toString()));
    }
    urls_list->setModel(urls_model);
    urls_del_btn=new QPushButton("删除",this);
    urls_edit=new QLineEdit(this);
    urls_add_btn=new QPushButton("增加",this);
    color_group=new QGroupBox("颜色",this);
    color_show=new QLabel(this);
    green_label=new QLabel("G",this);
    green_label->setAlignment(Qt::AlignCenter);
    red_label=new QLabel("R",this);
    red_label->setAlignment(Qt::AlignCenter);
    blue_label=new QLabel("B",this);
    blue_label->setAlignment(Qt::AlignCenter);
    QColor temp_color=qvariant_cast<QColor>(data.color);
    red_edit=new QLineEdit(QString::number(temp_color.redF()),this);
    green_edit=new QLineEdit(QString::number(temp_color.greenF()),this);
    blue_edit=new QLineEdit(QString::number(temp_color.blueF()),this);
    on_color_changed(temp_color);
    ok_btn=new QPushButton("确定",this);
    cancel_btn=new QPushButton("取消",this);



    QHBoxLayout *text_layout=new QHBoxLayout;
    text_layout->addWidget(text_edit);
    text_group->setLayout(text_layout);



    QHBoxLayout *html_layout=new QHBoxLayout;
    html_layout->addWidget(html_edit_group);
    html_layout->addWidget(html_show_group);
    QHBoxLayout *html_edit_layout=new QHBoxLayout;
    QHBoxLayout *html_show_layout=new QHBoxLayout;
    html_edit_group->setLayout(html_edit_layout);
    html_show_group->setLayout(html_show_layout);
    html_edit_layout->addWidget(html_edit);
    html_show_layout->addWidget(html_show);
    html_group->setLayout(html_layout);

    QGridLayout *image_layout=new QGridLayout;
    image_layout->addWidget(image_show,0,0,1,6);
    image_layout->addWidget(image_path_tip,1,0,1,1);
    image_layout->addWidget(image_path,1,1,1,2);
    image_layout->addWidget(get_image_path_btn,1,3,1,1);
    image_layout->addWidget(image_clear_btn,1,4,1,1);
    image_layout->addWidget(save_current_image,1,5,1,1);
    image_group->setLayout(image_layout);


    QGridLayout *urls_layout=new QGridLayout;
    urls_layout->addWidget(urls_list,0,0,1,5);
    urls_layout->addWidget(urls_del_btn,1,0,1,1);
    urls_layout->addWidget(urls_edit,1,1,1,3);
    urls_layout->addWidget(urls_add_btn,1,4,1,1);
    urls_group->setLayout(urls_layout);


    QHBoxLayout *color_layout=new QHBoxLayout;
    color_layout->addWidget(color_show);
    color_layout->addWidget(red_label);
    color_layout->addWidget(red_edit);
    color_layout->addWidget(green_label);
    color_layout->addWidget(green_edit);
    color_layout->addWidget(blue_label);
    color_layout->addWidget(blue_edit);
    color_group->setLayout(color_layout);

    QHBoxLayout *exit_layout=new QHBoxLayout;
    exit_layout->addWidget(ok_btn);
    exit_layout->addWidget(cancel_btn);


    QGridLayout *main_layout=new QGridLayout;

    main_layout->addWidget(text_group,0,0,1,1);
    main_layout->addWidget(html_group,1,0,4,1);
    main_layout->addWidget(image_group,0,1,1,1);
    main_layout->addWidget(urls_group,2,1,1,1);
    main_layout->addWidget(color_group,3,1,1,1);
    main_layout->addLayout(exit_layout,4,1,1,1);

    setLayout(main_layout);

    setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint);


    connect(html_edit,SIGNAL(textChanged()),this,SLOT(on_html_chnaged()));
    connect(get_image_path_btn,SIGNAL(clicked(bool)),this,SLOT(on_image_path_btn_clicked()));
    connect(image_clear_btn,SIGNAL(clicked(bool)),this,SLOT(on_image_clear()));
    connect(red_edit,SIGNAL(textChanged(QString)),this,SLOT(on_color_value_changed()));
    connect(green_edit,SIGNAL(textChanged(QString)),this,SLOT(on_color_value_changed()));
    connect(blue_edit,SIGNAL(textChanged(QString)),this,SLOT(on_color_value_changed()));
    connect(urls_del_btn,SIGNAL(clicked(bool)),this,SLOT(on_del_urls()));
    connect(urls_add_btn,SIGNAL(clicked(bool)),this,SLOT(on_add_urls()));
    connect(ok_btn,SIGNAL(clicked(bool)),this,SLOT(on_ok()));
    connect(cancel_btn,SIGNAL(clicked(bool)),this,SLOT(on_cancel()));
    connect(save_current_image,SIGNAL(clicked(bool)),this,SLOT(on_save_image()));
}

void DataEditWnd::on_color_changed(QColor color)
{
   color_show->setAutoFillBackground(true);
   QPalette palette;
   palette.setColor(QPalette::Background, color);
   color_show->setPalette(palette);
}


void DataEditWnd::on_html_chnaged(){
    html_show->setHtml(html_edit->toPlainText());
}

void DataEditWnd::on_image_path_btn_clicked(){
    QString file_name=QFileDialog::getOpenFileName(this,"打开图片","","图片(*.png *.xpm *.jpg *.bmp)");
    if(file_name.isEmpty())
        return;
    image_path->setText(file_name);
    QPixmap pix(file_name);
    image_show->setPixmap(pix.scaled(600,400,Qt::KeepAspectRatio));
}

void DataEditWnd::on_color_value_changed(){
    color_changed=true;
    on_color_changed(QColor(red_edit->text().toDouble(),green_edit->text().toDouble(),blue_edit->text().toDouble()));
}


void DataEditWnd::on_del_urls(){
    int index=urls_list->currentIndex().row();
    if(index<0||index>=urls_model->rowCount())
        return;
    urls_model->removeRow(index);
}

void DataEditWnd::on_add_urls(){
    QString url=urls_edit->text();
    if(url.isEmpty())
        return;
    urls_model->appendRow(new QStandardItem(url));
    urls_edit->clear();
}


void DataEditWnd::on_ok(){
    data.text=text_edit->toPlainText();
    if(!data.text.isEmpty()){
        data.type|=TEXT;
    }else{
        data.type&=~TEXT;
    }
    data.html=html_edit->toPlainText();
    if(!data.html.isEmpty()){
        data.type|=HTML;
    }else{
        data.type&=~HTML;
    }
    if(!image_path->text().isEmpty()){
        if(image_path->text()=="NULL"){
            data.image.clear();
            data.type&=~IMAGE;
        }else{
            QPixmap pixmap=QPixmap(image_path->text());
            data.image=QVariant(pixmap.toImage());
            data.type|=IMAGE;
        }
    }
    data.urls.clear();
    for(int i=0;i<urls_model->rowCount();++i){
        data.urls.append(QUrl(urls_model->takeItem(i)->text()));
    }
    if(!data.urls.isEmpty()){
        data.type|=URLS;
    }else{
        data.type&=~URLS;
    }
    if(color_changed){
        data.color=QVariant(QColor(red_edit->text().toDouble(),green_edit->text().toDouble(),blue_edit->text().toDouble()));
        data.type|=COLOR;
    }
    done(1);
}

void DataEditWnd::on_cancel(){
    done(0);
}

void DataEditWnd::on_image_clear(){
    QPixmap pixmap;
    image_show->setPixmap(pixmap);
    image_path->setText("NULL");
}

void DataEditWnd::on_save_image(){
    if(image_path->text().isEmpty()&&data.image.isNull()){
        QMessageBox::warning(this,"警告","没有图片数据");
        return;
    }
    QString file_name=QFileDialog::getSaveFileName(this,"另存","","图片(*.png *.xpm *.jpg *.bmp)");
    if(file_name.isEmpty())
        return;
    QString end_str=file_name.right(4).toLower();
    if(end_str!=".png"&&end_str!=".xpm"&&end_str!=".jpg"&&end_str!=".bmp")
        file_name+=".png";
    if(image_path->text().isEmpty()){
        if(!QPixmap::fromImage(qvariant_cast<QImage>(data.image)).save(file_name)){
            QMessageBox::warning(this,"警告","文件另存失败");
        }
    }else{
        if(!QPixmap(image_path->text()).save(file_name)){
            QMessageBox::warning(this,"警告","文件另存失败");
        }
    }
}

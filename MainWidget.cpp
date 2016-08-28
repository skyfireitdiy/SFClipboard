#include "MainWidget.h"
#include <Global.h>
#include <QDebug>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStandardItem>
#include <QFileDialog>
#include <DataEditWnd.h>
#include <TypeFilter.h>
#include <QMessageBox>

#include <QDesktopWidget>

static const int list_margin=30;
static const int menu_height=220;
static const int window_width=400;
static const int max_height=600;


MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    pClipContent=new ClipBoardContent(this);
    pView=new QListView(this);
    pModel=new QStandardItemModel(this);
    pView->setModel(pModel);
    QGridLayout *top_layout=new QGridLayout;
    QVBoxLayout *main_layout=new QVBoxLayout;

    enable_watch=new QCheckBox("开启监控",this);
    top_layout->addWidget(enable_watch,0,0,1,1);


    filter_btn=new QPushButton("类型过滤",this);
    top_layout->addWidget(filter_btn,0,1,1,1);


    edit_btn=new QPushButton("浏览/编辑数据",this);
    top_layout->addWidget(edit_btn,0,2,1,1);


    delete_record_btn=new QPushButton("删除记录",this);
    top_layout->addWidget(delete_record_btn,0,3,1,1);



    enable_auto_save=new QCheckBox("自动保存",this);
    auto_save_file_tip=new QLabel("文件路径",this);
    auto_save_file_tip->setAlignment(Qt::AlignCenter);
    auto_save_file_name=new QLineEdit(this);
    auto_save_file_name->setReadOnly(true);
    auto_save_file_name_btn=new QPushButton("浏览",this);

    top_layout->addWidget(enable_auto_save,1,0,1,1);
    top_layout->addWidget(auto_save_file_tip,1,1,1,1);
    top_layout->addWidget(auto_save_file_name,1,2,1,1);
    top_layout->addWidget(auto_save_file_name_btn,1,3,1,1);


    record_count_tip=new QLabel("历史记录数量(0表示不限制)",this);
    record_count_tip->setAlignment(Qt::AlignCenter);
    record_count_edit=new QLineEdit(this);
    record_count_set_btn=new QPushButton("设置数量",this);
    top_layout->addWidget(record_count_tip,2,0,1,2);
    top_layout->addWidget(record_count_edit,2,2,1,1);
    top_layout->addWidget(record_count_set_btn,2,3,1,1);


    save_to_file=new QPushButton("保存到文件",this);
    load_from_file=new QPushButton("从文件加载",this);
    export_image=new QPushButton("导出图片",this);
    export_urls=new QPushButton("导出URL",this);
    top_layout->addWidget(save_to_file,3,0,1,1);
    top_layout->addWidget(load_from_file,3,1,1,1);
    top_layout->addWidget(export_image,3,2,1,1);
    top_layout->addWidget(export_urls,3,3,1,1);


    export_text=new QPushButton("导出文本",this);
    export_text_one=new QPushButton("导出文本(单文件)",this);
    export_html=new QPushButton("导出HTML",this);
    export_html_one=new QPushButton("导出HTML(单文件)",this);
    top_layout->addWidget(export_text,4,0,1,1);
    top_layout->addWidget(export_text_one,4,1,1,1);
    top_layout->addWidget(export_html,4,2,1,1);
    top_layout->addWidget(export_html_one,4,3,1,1);


    main_layout->addWidget(pView);
    main_layout->addLayout(top_layout);
    setLayout(main_layout);

    pView->setFixedHeight(list_margin);
    setFixedHeight(menu_height);

    read_setting();


    connect(pClipContent,SIGNAL(data_changed(QQueue<Data>)),this,SLOT(on_data_changed(QQueue<Data>)));
    connect(pView,SIGNAL(clicked(QModelIndex)),this,SLOT(on_item_clicked(QModelIndex)));
    connect(this,SIGNAL(item_need_edit(int)),pClipContent,SLOT(on_data_edit(int)));
    connect(pClipContent,SIGNAL(edit_data(Data,int)),this,SLOT(on_edit_data(Data,int)));
    connect(this,SIGNAL(set_data(Data,int)),pClipContent,SLOT(on_set_data(Data,int)));
    connect(this,SIGNAL(item_clicked(int)),pClipContent,SLOT(on_set_clip_data(int)));
    connect(this,SIGNAL(setting_changed()),pClipContent,SLOT(read_setting()));
    connect(this,SIGNAL(delete_record(int)),pClipContent,SLOT(on_delete_record(int)));

    connect(enable_watch,SIGNAL(stateChanged(int)),this,SLOT(on_setting_changed()));
    connect(filter_btn,SIGNAL(clicked(bool)),this,SLOT(on_filter_btn_clicked()));
    connect(edit_btn,SIGNAL(clicked(bool)),this,SLOT(on_edit_btn_clicked()));
    connect(delete_record_btn,SIGNAL(clicked(bool)),this,SLOT(on_delete_record()));
    connect(enable_auto_save,SIGNAL(stateChanged(int)),this,SLOT(on_setting_changed()));
    connect(auto_save_file_name_btn,SIGNAL(clicked(bool)),this,SLOT(on_set_auto_save_file()));
    connect(save_to_file,SIGNAL(clicked(bool)),this,SLOT(on_save_to_file()));
    connect(this,SIGNAL(save_to_file_sgn(QString)),pClipContent,SLOT(on_save_to_file(QString)));
    connect(load_from_file,SIGNAL(clicked(bool)),this,SLOT(on_load_from_file()));
    connect(this,SIGNAL(load_from_file_sgn(QString)),pClipContent,SLOT(on_load_from_file(QString)));
    connect(record_count_set_btn,SIGNAL(clicked(bool)),this,SLOT(on_setting_changed()));
    connect(export_image,SIGNAL(clicked(bool)),this,SLOT(on_export_image()));
    connect(this,SIGNAL(export_image_sgn(QString)),pClipContent,SLOT(on_export_image(QString)));
    connect(export_urls,SIGNAL(clicked(bool)),this,SLOT(on_export_urls()));
    connect(this,SIGNAL(export_urls_sgn(QString)),pClipContent,SLOT(on_export_urls(QString)));
    connect(export_text,SIGNAL(clicked(bool)),this,SLOT(on_export_text()));
    connect(this,SIGNAL(export_text_sgn(QString)),pClipContent,SLOT(on_export_text(QString)));
    connect(export_text_one,SIGNAL(clicked(bool)),this,SLOT(on_export_text_single()));
    connect(this,SIGNAL(export_text_one_sgn(QString)),pClipContent,SLOT(on_export_text_single(QString)));
    connect(export_html,SIGNAL(clicked(bool)),this,SLOT(on_export_html()));
    connect(this,SIGNAL(export_html_sgn(QString)),pClipContent,SLOT(on_export_html(QString)));
    connect(export_html_one,SIGNAL(clicked(bool)),this,SLOT(on_export_html_single()));
    connect(this,SIGNAL(export_html_one_sgn(QString)),pClipContent,SLOT(on_export_html_single(QString)));
}


void MainWidget::on_data_changed(QQueue<Data> data){
    pModel->clear();
    int new_height=data.size()*pView->fontMetrics().height();
    if(new_height>=max_height)
        new_height=max_height;
    pView->setFixedHeight(new_height+list_margin);
    setFixedHeight(new_height+menu_height);
    for(int i=0;i<data.size();++i){
        QStandardItem *tempItem= new QStandardItem(QString::number(i)+" : "+ get_abstract(data.at(i)));
        tempItem->setEditable(false);
        pModel->insertRow(i,tempItem);
    }
}


QString MainWidget::get_abstract(const Data &data) const{
    auto make_short=[=](QString str){
        str=str.replace("\n","\\n");
        int str_width=this->pView->fontMetrics().width(str);
        int width=this->pView->width();
        if(str_width<width)
            return str;
        return str.left(str.length()/(str_width/width))+"...";
    };
    if(data.type&TEXT){
        return make_short(data.text);
    }else if(data.type&HTML){
        return make_short(data.html);
    }else if(data.type&IMAGE){
        return "<图片>";
    }else if(data.type&URLS){
        return make_short(data.urls.at(0).toString());
    }else if(data.type&COLOR){
        return "<颜色>";
    }else{
        return "<无数据>";
    }
}


void MainWidget::on_item_clicked(QModelIndex index){
    emit item_clicked(index.row());
}


void MainWidget::on_setting_changed(){
    pSettings->setValue("enable",enable_watch->isChecked());
    pSettings->setValue("auto_save",enable_auto_save->isChecked());
    auto_save_file_name->setEnabled(enable_auto_save->isChecked());
    auto_save_file_name_btn->setEnabled(enable_auto_save->isChecked());
    pSettings->setValue("auto_save_file",auto_save_file_name->text());
    pSettings->setValue("max_record_count",record_count_edit->text().toInt());
    emit setting_changed();
}


void MainWidget::read_setting(){
    enable_watch->setChecked(pSettings->value("enable",true).toBool());
    enable_auto_save->setChecked(pSettings->value("auto_save",false).toBool());
    auto_save_file_name->setEnabled(enable_auto_save->isChecked());
    auto_save_file_name_btn->setEnabled(enable_auto_save->isChecked());
    auto_save_file_name->setText(pSettings->value("auto_save_file","").toString());
    record_count_edit->setText(QString::number(pSettings->value("max_record_count",0).toInt()));
}

void MainWidget::on_filter_btn_clicked(){
    TypeFilter type;
    if(type.exec()){
        int data_type=0;
        if(type.enable_color->isChecked())
            data_type|=COLOR;
        if(type.enable_html->isChecked())
            data_type|=HTML;
        if(type.enable_image->isChecked())
            data_type|=IMAGE;
        if(type.enable_text->isChecked())
            data_type|=TEXT;
        if(type.enable_url->isChecked())
            data_type|=URLS;
        pSettings->setValue("data_type",data_type);
        emit setting_changed();
    }
}

void MainWidget::on_edit_data(Data data, int index){
    DataEditWnd data_edit_wnd(data);
    data_edit_wnd.exec();
    emit set_data(data_edit_wnd.data,index);
}

void MainWidget::on_edit_btn_clicked(){
     emit item_need_edit(pView->currentIndex().row());
}

void MainWidget::on_delete_record(){
    emit delete_record(pView->currentIndex().row());
}


void MainWidget::on_set_auto_save_file(){
    QString file_name=QFileDialog::getSaveFileName(this,"自动保存","","SFClipboard文件 (*.sfclp)");
    if(file_name.isEmpty())
        return;
    if(file_name.right(6).toLower()!=".sfclp")
        file_name+=".sfclp";
    auto_save_file_name->setText(file_name);
    on_setting_changed();
}


void MainWidget::on_save_to_file(){
    QString file_name=QFileDialog::getSaveFileName(this,"保存","","SFClipboard文件 (*.sfclp)");
    if(file_name.isEmpty())
        return;
    if(file_name.right(6).toLower()!=".sfclp")
        file_name+=".sfclp";
    emit save_to_file_sgn(file_name);
}


void MainWidget::on_load_from_file(){
    QString file_name=QFileDialog::getOpenFileName(this,"保存","","SFClipboard文件 (*.sfclp)");
    if(file_name.isEmpty())
        return;
    record_count_edit->setText("0");
    on_setting_changed();
    emit load_from_file_sgn(file_name);
}


void MainWidget::on_export_image(){
    QString dir_name=QFileDialog::getExistingDirectory(this,"导出目录","", QFileDialog::DontResolveSymlinks);
    if(dir_name.isEmpty())
        return;
    emit export_image_sgn(dir_name);
}

void MainWidget::on_export_urls(){
    QString file_name=QFileDialog::getSaveFileName(this,"保存","","文本文件 (*.txt)");
    if(file_name.isEmpty())
        return;
    if(file_name.right(4).toLower()!=".txt")
        file_name+=".txt";
    emit export_urls_sgn(file_name);
}


void MainWidget::on_export_text(){
    QString dir_name=QFileDialog::getExistingDirectory(this,"导出目录","", QFileDialog::DontResolveSymlinks);
    if(dir_name.isEmpty())
        return;
    emit export_text_sgn(dir_name);
}

void MainWidget::on_export_text_single(){
    QString file_name=QFileDialog::getSaveFileName(this,"保存","","文本文件 (*.txt)");
    if(file_name.isEmpty())
        return;
    if(file_name.right(4).toLower()!=".txt")
        file_name+=".txt";
    emit export_text_one_sgn(file_name);
}


void MainWidget::on_export_html(){
    QString dir_name=QFileDialog::getExistingDirectory(this,"导出目录","", QFileDialog::DontResolveSymlinks);
    if(dir_name.isEmpty())
        return;
    emit export_html_sgn(dir_name);
}

void MainWidget::on_export_html_single(){
    QString file_name=QFileDialog::getSaveFileName(this,"保存","","网页文件 (*.html)");
    if(file_name.isEmpty())
        return;
    if(file_name.right(5).toLower()!=".html")
        file_name+=".html";
    emit export_html_one_sgn(file_name);
}

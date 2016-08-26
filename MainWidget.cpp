#include "MainWidget.h"
#include <Global.h>
#include <QDebug>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStandardItem>
#include <QFileDialog>
#include <DataEditWnd.h>
#include <TypeFilter.h>

static const int list_margin=30;
static const int menu_height=100;
static const int window_width=400;


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
    auto_save_file_name=new QLineEdit(this);
    auto_save_file_name->setReadOnly(true);
    auto_save_file_name_btn=new QPushButton("浏览",this);

    top_layout->addWidget(enable_auto_save,1,0,1,1);
    top_layout->addWidget(auto_save_file_tip,1,1,1,1);
    top_layout->addWidget(auto_save_file_name,1,2,1,1);
    top_layout->addWidget(auto_save_file_name_btn,1,3,1,1);



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



}


void MainWidget::on_data_changed(QQueue<Data> data){
    pModel->clear();
    pView->setFixedHeight(data.size()*pView->fontMetrics().height()+list_margin);
    setFixedHeight(data.size()*pView->fontMetrics().height()+menu_height);
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
    emit setting_changed();
}


void MainWidget::read_setting(){
    enable_watch->setChecked(pSettings->value("enable",true).toBool());
    enable_auto_save->setChecked(pSettings->value("auto_save",false).toBool());
    auto_save_file_name->setEnabled(enable_auto_save->isChecked());
    auto_save_file_name_btn->setEnabled(enable_auto_save->isChecked());
    auto_save_file_name->setText(pSettings->value("auto_save_file","").toString());
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
    QString file_name=QFileDialog::getSaveFileName(this,"自动保存","","SFClipboard 保存文件 (*.sfclp)");
    if(file_name.isEmpty())
        return;
    if(file_name.right(6).toLower()!=".sfclp")
        file_name+=".sfclp";
    auto_save_file_name->setText(file_name);
    on_setting_changed();
}

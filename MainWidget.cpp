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
#include <QIcon>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QBitmap>

#ifdef _WIN32
#include <windows.h>
#include <WinHook.h>
#endif

static const int list_margin=30;
static const int menu_height=260;
static const int window_width=440;
static const int max_height=400;

#define NEED_NO_TOP(x) setWindowFlags(windowFlags()&~Qt::WindowStaysOnTopHint);x;setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint);resizeEvent(0);show();


MainWidget::MainWidget(QWidget *parent) : QWidget(parent),auto_hide_widget(0),real_exit(false),auto_hide(false)
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


    about_btn=new QPushButton("关于",this);
    clear_all=new QPushButton("清空",this);
    rel_close=new QPushButton("关闭");
    hide_btn=new QPushButton("",this);
    top_layout->addWidget(about_btn,5,0,1,1);
    top_layout->addWidget(clear_all,5,1,1,1);
    top_layout->addWidget(rel_close,5,2,1,1);
    top_layout->addWidget(hide_btn,5,3,1,1);


    main_layout->addWidget(pView);
    main_layout->addLayout(top_layout);
    setLayout(main_layout);

    pView->setFixedHeight(list_margin);
    setFixedHeight(menu_height);

    setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::X11BypassWindowManagerHint);

    setContentsMargins(50,20,50,20);
    read_setting();

    tray_menu=new QMenu(this);
    tray_menu->addAction("显示/隐藏主界面",this,SLOT(on_show_hide_widget()));
    tray_menu->addAction("退出",this,SLOT(on_real_exit()));
    tray_icon=new QSystemTrayIcon(QIcon(":/icon/resource/tray.png"),this);
    tray_icon->show();
    tray_icon->setContextMenu(tray_menu);

    setContextMenuPolicy(Qt::CustomContextMenu);

    setWindowIcon(QIcon(":/icon/resource/tray.png"));
    setStyleSheet(
                  "*{font-weight:bold;color:white;background-color:#220000;}"
                  ".QListView{color:white;background-color:#220000;}"
                  ".QPushButton{border-image:url(:/pic/resource/btn.png);border-width:5px;}"
                  ".QPushButton:hover{border-image:url(:/pic/resource/btn_hover.png);border-width:5px;}"
                  ".QPushButton:pressed{border-image:url(:/pic/resource/btn_pressed.png);border-width:5px;}"
                  ".QMenu{background-color:#110000;}"
                  ".QMessageBox{border-image:url(:/pic/resource/background.png);"
                  ".QPlainTextEdit{background-color:#220000;}"
                  ".QTextBrowser{background-color:#220000;"
                  " .QLineEdit{background-color:#220000;}"
                  );


    setWindowOpacity(0.9);

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
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_customContextMenuRequested()));
    connect(clear_all,SIGNAL(clicked(bool)),this,SLOT(on_clear_all()));
    connect(this,SIGNAL(clear_all_sgn()),pClipContent,SLOT(on_clear_all()));
    connect(about_btn,SIGNAL(clicked(bool)),this,SLOT(on_about()));
    connect(rel_close,SIGNAL(clicked(bool)),this,SLOT(on_real_exit()));
    connect(hide_btn,SIGNAL(clicked(bool)),this,SLOT(on_auto_hide()));

#ifdef _WIN32
    setHook();
#endif

}


void MainWidget::on_data_changed(QQueue<Data> data){
    pModel->clear();
    for(int i=0;i<data.size();++i){
        QStandardItem *tempItem= new QStandardItem(QString::number(i)+" : "+ get_abstract(data.at(i)));
        tempItem->setEditable(false);
        pModel->insertRow(i,tempItem);
    }
    int new_height=data.size()*(pView->fontMetrics().lineSpacing()+2);
    if(new_height>=max_height)
        new_height=max_height;
    pView->setFixedHeight(new_height+list_margin);
    setFixedHeight(new_height+menu_height);
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
    write_setting();
    emit setting_changed();
    flush_settings();
}

void MainWidget::write_setting(){
    pSettings->setValue("enable",enable_watch->isChecked());
    pSettings->setValue("auto_save",enable_auto_save->isChecked());
    pSettings->setValue("auto_save_file",auto_save_file_name->text());
    pSettings->setValue("max_record_count",record_count_edit->text().toInt());
    pSettings->setValue("auto_hide",auto_hide);
    pSettings->setValue("pos",pos());
}


void MainWidget::read_setting(){
    enable_watch->setChecked(pSettings->value("enable",true).toBool());
    enable_auto_save->setChecked(pSettings->value("auto_save",false).toBool());
    auto_save_file_name->setEnabled(enable_auto_save->isChecked());
    auto_save_file_name_btn->setEnabled(enable_auto_save->isChecked());
    auto_save_file_name->setText(pSettings->value("auto_save_file","").toString());
    record_count_edit->setText(QString::number(pSettings->value("max_record_count",0).toInt()));
    auto_hide=pSettings->value("auto_hide",true).toBool();
    move(pSettings->value("pos",QPoint(0,0)).toPoint());
    flush_settings();
}

void MainWidget::on_filter_btn_clicked(){
    TypeFilter type;
    NEED_NO_TOP(
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
    );
}

void MainWidget::on_edit_data(Data data, int index){
    DataEditWnd data_edit_wnd(data);
    NEED_NO_TOP(data_edit_wnd.exec());
    emit set_data(data_edit_wnd.data,index);
}

void MainWidget::on_edit_btn_clicked(){
     emit item_need_edit(pView->currentIndex().row());
}

void MainWidget::on_delete_record(){
    emit delete_record(pView->currentIndex().row());
}


void MainWidget::on_set_auto_save_file(){
   NEED_NO_TOP(QString file_name=QFileDialog::getSaveFileName(this,"自动保存","","SFClipboard文件 (*.sfclp)"));
    if(file_name.isEmpty())
        return;
    if(file_name.right(6).toLower()!=".sfclp")
        file_name+=".sfclp";
    auto_save_file_name->setText(file_name);
    on_setting_changed();
}


void MainWidget::on_save_to_file(){
    NEED_NO_TOP(QString file_name=QFileDialog::getSaveFileName(this,"保存","","SFClipboard文件 (*.sfclp)"));
    if(file_name.isEmpty())
        return;
    if(file_name.right(6).toLower()!=".sfclp")
        file_name+=".sfclp";
    emit save_to_file_sgn(file_name);
}


void MainWidget::on_load_from_file(){
    NEED_NO_TOP(QString file_name=QFileDialog::getOpenFileName(this,"保存","","SFClipboard文件 (*.sfclp)"));
    if(file_name.isEmpty())
        return;
    record_count_edit->setText("0");
    on_setting_changed();
    emit load_from_file_sgn(file_name);
}


void MainWidget::on_export_image(){
    NEED_NO_TOP(QString dir_name=QFileDialog::getExistingDirectory(this,"导出目录","", QFileDialog::DontResolveSymlinks));
    if(dir_name.isEmpty())
        return;
    emit export_image_sgn(dir_name);
}

void MainWidget::on_export_urls(){
    NEED_NO_TOP( QString file_name=QFileDialog::getSaveFileName(this,"保存","","文本文件 (*.txt)"));
    if(file_name.isEmpty())
        return;
    if(file_name.right(4).toLower()!=".txt")
        file_name+=".txt";
    emit export_urls_sgn(file_name);
}


void MainWidget::on_export_text(){
    NEED_NO_TOP(QString dir_name=QFileDialog::getExistingDirectory(this,"导出目录","", QFileDialog::DontResolveSymlinks));
    if(dir_name.isEmpty())
        return;
    emit export_text_sgn(dir_name);
}

void MainWidget::on_export_text_single(){
    NEED_NO_TOP(QString file_name=QFileDialog::getSaveFileName(this,"保存","","文本文件 (*.txt)"));
    if(file_name.isEmpty())
        return;
    if(file_name.right(4).toLower()!=".txt")
        file_name+=".txt";
    emit export_text_one_sgn(file_name);
}


void MainWidget::on_export_html(){
    NEED_NO_TOP(QString dir_name=QFileDialog::getExistingDirectory(this,"导出目录","", QFileDialog::DontResolveSymlinks));
    if(dir_name.isEmpty())
        return;
    emit export_html_sgn(dir_name);
}

void MainWidget::on_export_html_single(){
    NEED_NO_TOP( QString file_name=QFileDialog::getSaveFileName(this,"保存","","网页文件 (*.html)"));
    if(file_name.isEmpty())
        return;
    if(file_name.right(5).toLower()!=".html")
        file_name+=".html";
    emit export_html_one_sgn(file_name);
}

void MainWidget::on_show_hide_widget(){
    if(isHidden()){
        show();
    }else{
        hide();
    }
}

void MainWidget::closeEvent(QCloseEvent *event){
    hide();
    if(!real_exit)
        event->ignore();
    else
        event->accept();
}

void MainWidget::on_real_exit(){
    NEED_NO_TOP(auto ret=QMessageBox::question(this,"退出","是否退出SFClipboard?",QMessageBox::No,QMessageBox::Yes));
    if(ret==QMessageBox::No){
        return;
    }
    real_exit=true;
    close();
}

void MainWidget::on_customContextMenuRequested(){
    tray_menu->popup(QCursor::pos());
}

void MainWidget::resizeEvent(QResizeEvent *){
    QPalette pal;
    pal.setBrush(QPalette::Window,QBrush(QPixmap(":/pic/resource/background.png").scaled(size())));
    setPalette(pal);

    QRect widget_rect=geometry();
    QRect desktop_rect=pApp->desktop()->geometry();
    if(!auto_hide_widget&&widget_rect.bottom()>desktop_rect.bottom()){
        move(widget_rect.x(),desktop_rect.bottom()-widget_rect.height());
    }
    emit rect_changed();
}

void MainWidget::on_about(){
    NEED_NO_TOP(QMessageBox::about(this,"关于SFClipboard","作者：SkyFire\nQQ：1513008876\nE-mail：skyfireitdiy@hotmail.com\n项目地址：http://git.oschina.net/skyfireitdiy/SFClipboard\n版本："+version+"\n编译时间："+__DATE__+" "+__TIME__+"\n"));
}

void MainWidget::on_clear_all(){
    emit clear_all_sgn();
}

void MainWidget::on_auto_hide(){
    auto_hide=!auto_hide;
    on_setting_changed();
}

void MainWidget::flush_settings(){

    auto_save_file_name->setEnabled(enable_auto_save->isChecked());
    auto_save_file_name_btn->setEnabled(enable_auto_save->isChecked());

    if(auto_hide){
        if(auto_hide_widget){
            delete auto_hide_widget;
        }
        auto_hide_widget=new AutoHide(this);
        hide_btn->setText("固定");
    }else{
        if(auto_hide_widget){
            delete auto_hide_widget;
            auto_hide_widget=0;
        }
        hide_btn->setText("悬浮");
    }
}

MainWidget::~MainWidget(){
    write_setting();

#ifdef _WIN32
    unHook();
#endif

}

#ifdef _WIN32

void MainWidget::on_hot_key(int num){
    if(num<pModel->rowCount()){
        emit item_clicked(num);
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('V',0,0,0);
        keybd_event('V',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
    }
}

#endif

#undef NEED_NO_TOP

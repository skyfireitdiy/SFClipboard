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
#include <thread>
#include <QEventLoop>
#include <SFLanguage.h>
#include <QProcess>
#include <SFReg.h>
#include <QDesktopWidget>
#ifdef _WIN32
#include <windows.h>
#include <WinHook.h>
#else
#ifdef __unix
#include <linux/input.h>
#include <unistd.h>
#include <LinuxHook.h>
#endif
#endif

//#define FOR_FF

SFLanguage *__lan_st=nullptr;

#define _1000_MS 1000

static const int list_margin=30;
static const int menu_height=200;
static const int window_width=340;
static const int max_height=300;

#define NEED_NO_TOP(x) setWindowFlags(windowFlags()&~Qt::WindowStaysOnTopHint);x;setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint);resizeEvent(0);show();


MainWidget::MainWidget(QWidget *parent) : QWidget(parent),auto_hide_widget(0),real_exit(false),auto_hide(false)
{
    pView=new QListView(this);
    pModel=new QStandardItemModel(this);
    pView->setModel(pModel);
    pView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QGridLayout *top_layout=new QGridLayout;
    QVBoxLayout *main_layout=new QVBoxLayout;


    enable_watch=new QCheckBox(GS("START_MONITOR"),this);
    top_layout->addWidget(enable_watch,0,0,1,1);

    clear_all=new QPushButton(GS("CLEAR"),this);
    top_layout->addWidget(clear_all,0,1,1,1);

    edit_btn=new QPushButton(GS("WATCH_EDIT_DATA"),this);
    top_layout->addWidget(edit_btn,0,2,1,1);


    delete_record_btn=new QPushButton(GS("DELETE_RECORD"),this);
    top_layout->addWidget(delete_record_btn,0,3,1,1);


    enable_auto_save=new QCheckBox(GS("AUTO_SAVE"),this);
    auto_save_file_tip=new QLabel(GS("FILE_PATH"),this);
    auto_save_file_tip->setAlignment(Qt::AlignCenter);
    auto_save_file_name=new QLineEdit(this);
    auto_save_file_name->setReadOnly(true);
    auto_save_file_name_btn=new QPushButton(GS("BROWSE"),this);

    top_layout->addWidget(enable_auto_save,1,0,1,1);
    top_layout->addWidget(auto_save_file_tip,1,1,1,1);
    top_layout->addWidget(auto_save_file_name,1,2,1,1);
    top_layout->addWidget(auto_save_file_name_btn,1,3,1,1);


    record_count_tip=new QLabel(GS("HIS_NUM"),this);
    record_count_tip->setAlignment(Qt::AlignCenter);
    record_count_edit=new QLineEdit(this);
    record_count_set_btn=new QPushButton(GS("SET_NUM"),this);
    top_layout->addWidget(record_count_tip,2,0,1,2);
    top_layout->addWidget(record_count_edit,2,2,1,1);
    top_layout->addWidget(record_count_set_btn,2,3,1,1);

    about_btn=new QPushButton(GS("ABOUT"),this);

    hide_btn=new QPushButton("",this);
    top_layout->addWidget(about_btn,3,0,1,1);
    top_layout->addWidget(hide_btn,3,3,1,1);

    main_layout->addWidget(pView);
    main_layout->addLayout(top_layout);
    setLayout(main_layout);

    pView->setFixedHeight(list_margin);
    setFixedHeight(menu_height);

    //setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint|Qt::X11BypassWindowManagerHint|Qt::Tool);

    setContentsMargins(10,10,10,10);


    tray_menu=new QMenu(this);

#ifdef __WIN32
    auto_run=tray_menu->addAction(GS("AUTO_RUN"),this,SLOT(on_auto_run()));
    auto_run->setCheckable(true);
#endif

    auto_load_auto_save_file_act=tray_menu->addAction(GS("AUTO_LOAD_AUTO_SAVE_FILE"),this,SLOT(on_auto_load_auto_save_file()));
    auto_load_auto_save_file_act->setCheckable(true);

    QMenu *lan_menu=tray_menu->addMenu("语言");
    QStringList lan_list=LAN_LIST;
    QList<QAction*> act_list;
    for(auto i:lan_list){
        QAction *act=new QAction(i,this);
        act_list.push_back(act);
    }
    lan_menu->addActions(act_list);
    connect(lan_menu,SIGNAL(triggered(QAction*)),this,SLOT(on_lang_set(QAction*)));


    tray_menu->addAction(GS("HIDE_SHOW_MAIN_WINDOW"),this,SLOT(on_show_hide_widget()));
    filter_act=tray_menu->addAction(GS("TYPE_FILTER"),this,SLOT(on_filter_act_clicked()));
    save_to_file=tray_menu->addAction(GS("SAVE_TO_FILE"),this,SLOT(on_save_to_file()));
    load_from_file =tray_menu->addAction(GS("LOAD_FROM_FILE"),this,SLOT(on_load_from_file()));
    export_image=tray_menu->addAction(GS("EXPORT_IMAGE"),this,SLOT(on_export_image()));
    export_text=tray_menu->addAction(GS("EXPORT_TEXT"),this,SLOT(on_export_text()));
    export_html=tray_menu->addAction(GS("EXPORT_HTML"),this,SLOT(on_export_html()));
    export_urls=tray_menu->addAction(GS("EXPORT_URL"),this,SLOT(on_export_urls()));
    export_text_one=tray_menu->addAction(GS("EXPORT_TEXT_SINGLE"),this,SLOT(on_export_text_single()));
    export_html=tray_menu->addAction(GS("EXPORT_HTML_SINGLE"),this,SLOT(on_export_html_single()));



    float_fix_act=tray_menu->addAction("",this,SLOT(on_auto_hide()));

    tray_menu->addAction(GS("EXIT"),this,SLOT(on_real_exit()));

    tray_menu->setWindowFlags(tray_menu->windowFlags()|Qt::WindowStaysOnTopHint);

    tray_icon=new QSystemTrayIcon(QIcon(":/icon/resource/tray.png"),this);
    tray_icon->show();
    tray_icon->setContextMenu(tray_menu);


    read_setting();

    setContextMenuPolicy(Qt::CustomContextMenu);

    setWindowIcon(QIcon(":/icon/resource/tray.png"));

    setWindowOpacity(0.9);
    pClipContent=new ClipBoardContent(this);
    connect(pClipContent,SIGNAL(data_changed(QQueue<Data>)),this,SLOT(on_data_changed(QQueue<Data>)));
    connect(this,SIGNAL(item_need_edit(int)),pClipContent,SLOT(on_data_edit(int)));
    connect(pClipContent,SIGNAL(edit_data(Data,int)),this,SLOT(on_edit_data(Data,int)));
    connect(this,SIGNAL(set_data(Data,int)),pClipContent,SLOT(on_set_data(Data,int)));
    connect(this,SIGNAL(item_clicked(int)),pClipContent,SLOT(on_set_clip_data(int)));
    connect(this,SIGNAL(setting_changed()),pClipContent,SLOT(read_setting()));
    connect(this,SIGNAL(delete_record(int)),pClipContent,SLOT(on_delete_record(int)));
    connect(this,SIGNAL(save_to_file_sgn(QString)),pClipContent,SLOT(on_save_to_file(QString)));
    connect(this,SIGNAL(load_from_file_sgn(QString)),pClipContent,SLOT(on_load_from_file(QString)));
    connect(this,SIGNAL(export_urls_sgn(QString)),pClipContent,SLOT(on_export_urls(QString)));
    connect(this,SIGNAL(export_text_one_sgn(QString)),pClipContent,SLOT(on_export_text_single(QString)));
    connect(this,SIGNAL(export_text_sgn(QString)),pClipContent,SLOT(on_export_text(QString)));
    connect(this,SIGNAL(export_html_sgn(QString)),pClipContent,SLOT(on_export_html(QString)));
    connect(this,SIGNAL(export_html_one_sgn(QString)),pClipContent,SLOT(on_export_html_single(QString)));
    connect(this,SIGNAL(export_image_sgn(QString)),pClipContent,SLOT(on_export_image(QString)));
    connect(this,SIGNAL(clear_all_sgn()),pClipContent,SLOT(on_clear_all()));



    connect(pView,SIGNAL(clicked(QModelIndex)),this,SLOT(on_item_clicked(QModelIndex)));
    connect(enable_watch,SIGNAL(stateChanged(int)),this,SLOT(on_setting_changed()));
    connect(edit_btn,SIGNAL(clicked(bool)),this,SLOT(on_edit_btn_clicked()));
    connect(delete_record_btn,SIGNAL(clicked(bool)),this,SLOT(on_delete_record()));
    connect(enable_auto_save,SIGNAL(stateChanged(int)),this,SLOT(on_setting_changed()));
    connect(auto_save_file_name_btn,SIGNAL(clicked(bool)),this,SLOT(on_set_auto_save_file()));
    connect(record_count_set_btn,SIGNAL(clicked(bool)),this,SLOT(on_setting_changed()));
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_customContextMenuRequested()));
    connect(clear_all,SIGNAL(clicked(bool)),this,SLOT(on_clear_all()));
    connect(about_btn,SIGNAL(clicked(bool)),this,SLOT(on_about()));
    connect(hide_btn,SIGNAL(clicked(bool)),this,SLOT(on_auto_hide()));
    connect(pView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_edit_btn_clicked()));
    connect(tray_icon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_tray_active(QSystemTrayIcon::ActivationReason)));
    connect(this,SIGNAL(frame_in_out_sgn()),this,SLOT(on_in_out()));



    if(auto_load_auto_save_file_act->isChecked()){
        emit load_from_file_sgn(auto_save_file_name->text());
    }
    setHook();
    on_tray_msg(GS("START"),GS("START_MSG"));
    pSettings->setValue("version",version);
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
        return "<"+GS("IMAGE")+">";
    }else if(data.type&URLS){
        return make_short(data.urls.at(0).toString());
    }else if(data.type&COLOR){
        return "<"+GS("COLOR")+">";
    }else{
        return "<"+GS("NO_DATA")+">";
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
    pSettings->setValue("auto_load_auto_save_file",auto_load_auto_save_file_act->isChecked());
#ifdef __WIN32
    pSettings->setValue("auto_run",auto_run->isChecked());
#endif
    pSettings->sync();
}


void MainWidget::read_setting(){
    enable_watch->setChecked(pSettings->value("enable",true).toBool());
    enable_auto_save->setChecked(pSettings->value("auto_save",false).toBool());
    auto_save_file_name->setEnabled(enable_auto_save->isChecked());
    auto_save_file_name_btn->setEnabled(enable_auto_save->isChecked());
    auto_save_file_name->setText(pSettings->value("auto_save_file","").toString());
    record_count_edit->setText(QString::number(pSettings->value("max_record_count",0).toInt()));
    auto_hide=pSettings->value("auto_hide",true).toBool();
    QPoint p=pSettings->value("pos",QPoint(0,0)).toPoint();
    QDesktopWidget *desktop= pApp->desktop();
    if(p.x()>desktop->width())
        p.setX(desktop->width()-50);
    if(p.y()>desktop->height())
        p.setY(desktop->height()-50);
    if(p.x()< -this->width())
        p.setX(0);
    if(p.y()< -this->height())
        p.setY(0);

    move(p);
#ifdef __WIN32
    auto_run->setChecked(pSettings->value("auto_run",false).toBool());
#endif
    auto_load_auto_save_file_act->setChecked(pSettings->value("auto_load_auto_save_file").toBool());
    flush_settings();
}

void MainWidget::on_filter_act_clicked(){
    TypeFilter type(this);
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
    DataEditWnd data_edit_wnd(data,this);
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
    NEED_NO_TOP(QString file_name=QFileDialog::getSaveFileName(this,GS("AUTO_SAVE"),"",GS("SFCLIPBOARD_FILE")+"(*.sfclp)"));
    if(file_name.isEmpty())
        return;
    if(file_name.right(6).toLower()!=".sfclp")
        file_name+=".sfclp";
    auto_save_file_name->setText(file_name);
    on_setting_changed();
    on_tray_msg(GS("AUTO_SAVE"),file_name);
}


void MainWidget::on_save_to_file(){
    NEED_NO_TOP(QString file_name=QFileDialog::getSaveFileName(this,GS("SAVE"),"",GS("SFCLIPBOARD_FILE")+"(*.sfclp)"));
    if(file_name.isEmpty())
        return;
    if(file_name.right(6).toLower()!=".sfclp")
        file_name+=".sfclp";
    emit save_to_file_sgn(file_name);
}


void MainWidget::on_load_from_file(){
    NEED_NO_TOP(QString file_name=QFileDialog::getOpenFileName(this,GS("SAVE"),"",GS("SFCLIPBOARD_FILE")+"(*.sfclp)"));
    if(file_name.isEmpty())
        return;
    record_count_edit->setText("0");
    on_setting_changed();
    emit load_from_file_sgn(file_name);
}


void MainWidget::on_export_image(){
    NEED_NO_TOP(QString dir_name=QFileDialog::getExistingDirectory(this,GS("EXPORT_DIR"),"", QFileDialog::DontResolveSymlinks));
    if(dir_name.isEmpty())
        return;
    emit export_image_sgn(dir_name);
}

void MainWidget::on_export_urls(){
    NEED_NO_TOP( QString file_name=QFileDialog::getSaveFileName(this,GS("SAVE"),"",GS("TEXT_FILE")+"(*.txt)"));
    if(file_name.isEmpty())
        return;
    if(file_name.right(4).toLower()!=".txt")
        file_name+=".txt";
    emit export_urls_sgn(file_name);
}


void MainWidget::on_export_text(){
    NEED_NO_TOP(QString dir_name=QFileDialog::getExistingDirectory(this,GS("EXPORT_DIR"),"", QFileDialog::DontResolveSymlinks));
    if(dir_name.isEmpty())
        return;
    emit export_text_sgn(dir_name);
}

void MainWidget::on_export_text_single(){
    NEED_NO_TOP(QString file_name=QFileDialog::getSaveFileName(this,GS("SAVE"),"",GS("TEXT_FILE")+"(*.txt)"));
    if(file_name.isEmpty())
        return;
    if(file_name.right(4).toLower()!=".txt")
        file_name+=".txt";
    emit export_text_one_sgn(file_name);
}


void MainWidget::on_export_html(){
    NEED_NO_TOP(QString dir_name=QFileDialog::getExistingDirectory(this,GS("EXPORT_DIR"),"", QFileDialog::DontResolveSymlinks));
    if(dir_name.isEmpty())
        return;
    emit export_html_sgn(dir_name);
}

void MainWidget::on_export_html_single(){
    NEED_NO_TOP( QString file_name=QFileDialog::getSaveFileName(this,GS("SAVE"),"",GS("HTML_FILE")+"(*.html)"));
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
    if(!real_exit){
        event->ignore();
        on_tray_msg(GS("CLOSE"),GS("HIDE_TO_TRAY"));
    }else{
        event->accept();
        tray_icon->hide();
        pSingleApp->detach();
        pApp->exit(0);
    }
}

void MainWidget::on_real_exit(){
    NEED_NO_TOP(auto ret=QMessageBox::question(this,GS("EXIT"),GS("IF_EXIT"),QMessageBox::No,QMessageBox::Yes));
    if(ret==QMessageBox::No){
        return;
    }
    real_exit=true;
    close();
}

void MainWidget::on_customContextMenuRequested(){
    tray_menu->setWindowFlags(tray_menu->windowFlags()|Qt::WindowStaysOnTopHint);
    tray_menu->popup(QCursor::pos());
}

void MainWidget::resizeEvent(QResizeEvent *){
    QRect widget_rect=geometry();
    QRect desktop_rect=pApp->desktop()->geometry();
    if(!auto_hide_widget&&widget_rect.bottom()>desktop_rect.bottom()){
        move(widget_rect.x(),desktop_rect.bottom()-widget_rect.height());
    }
    emit rect_changed();
}

void MainWidget::on_about(){
    QMessageBox about_wnd(this);
#ifdef FOR_FF
    about_wnd.setIconPixmap(QPixmap(":/pic/resource/about_icon.png"));
#else
    about_wnd.setIconPixmap(QPixmap(":/pic/resource/sponse.png"));
#endif
    about_wnd.setWindowIconText(GS("ABOUT_MESSAGE"));
    about_wnd.setText(GS("AUTHOR")+"：SkyFire\n"+GS("QQ")+"：1513008876\n"+GS("EMAIL")+"：skyfireitdiy@hotmail.com\n"+GS("PROJECT_ADDRESS")+"：http://git.oschina.net/skyfireitdiy/SFClipboard\n"+GS("VERSION")+"："+version+"\n"+GS("BUILD_TIME")+"："+__DATE__+" "+__TIME__+"\n\n\n"+GS("HELP_INFO"));
    NEED_NO_TOP(about_wnd.exec());
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
            auto_hide_widget=nullptr;
        }
        auto_hide_widget=new AutoHide(this);
        connect(this,SIGNAL(rect_changed()),auto_hide_widget,SLOT(on_check_if_hide()));
        connect(this,SIGNAL(frame_in_out_sgn()),auto_hide_widget,SLOT(on_in_out()));
        connect(auto_hide_widget,SIGNAL(pos_changed(QPoint)),this,SLOT(on_pos_chnaged(QPoint)));
        set_button_backimg(hide_btn,":/pic/resource/float.png");
        float_fix_act->setText(GS("FIXED"));
    }else{
        if(auto_hide_widget){
            delete auto_hide_widget;
            auto_hide_widget=nullptr;
        }
        set_button_backimg(hide_btn,":/pic/resource/fixed.png");
        float_fix_act->setText(GS("FLOAT"));
    }
}

void MainWidget::on_tray_active(QSystemTrayIcon::ActivationReason reason){
    if(reason==QSystemTrayIcon::DoubleClick){
        on_show_hide_widget();
    }
}

MainWidget::~MainWidget(){
    write_setting();
    unHook();
}


void MainWidget::on_hot_copy(int num){
    if(num<pModel->rowCount()){
        emit item_clicked(num);
#ifdef _WIN32
        keybd_event(VK_CONTROL,0,0,0);
        keybd_event('V',0,0,0);
        keybd_event('V',0,KEYEVENTF_KEYUP,0);
        keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);

#else
#ifdef __unix

        reportkey(KEY_RIGHTCTRL,1);
        reportkey(KEY_V,1);
        reportkey(KEY_V,0);
        reportkey(KEY_RIGHTCTRL,0);
#endif
#endif
    }
}

void MainWidget::on_hot_delete(int num){
    if(num<pModel->rowCount()){
        emit delete_record(num);
    }
}


void MainWidget::on_need_root(){
    NEED_NO_TOP(
                QMessageBox::warning(this,GS("WARNING"),GS("ROOT_REQUEST"),QMessageBox::Ok);
            );
}

void MainWidget::on_lang_set(QAction *act){
    if(act->text()==GET_CURR_LANG){
        return;
    }
    bool ret=false;
    NEED_NO_TOP(ret=QMessageBox::question(this,GS("WARNING"),GS("CHANGE_LAN_TIP"),QMessageBox::Ok,QMessageBox::Cancel)==QMessageBox::Ok);
    if(ret){
        SET_LANG(act->text());
        pSettings->setValue("lang",act->text());
        pSingleApp->detach();
        pSettings->sync();
        QProcess pro;
        pro.startDetached("\""+pApp->arguments().at(0)+"\"");
        tray_icon->hide();
        exit(0);
    }
}




void MainWidget::on_pos_chnaged(QPoint pos){
    pSettings->setValue("pos",pos);
    pSettings->sync();
}


void MainWidget::on_auto_run(){
#ifdef __WIN32
    if(auto_run->isChecked()){
        SFReg::add_win32_auto_run_once(programName,pApp->arguments()[0]);
    }else{
        SFReg::delete_win32_auto_run_once(programName);
    }
    write_setting();
#endif
}

void MainWidget::on_auto_load_auto_save_file(){
    write_setting();
}

void MainWidget::on_in_out(){
    if(!auto_hide){
        on_auto_hide();
        if(auto_hide_widget!=nullptr)
            auto_hide_widget->on_in_out();
    }
}


void MainWidget::on_tray_msg(QString title,QString msg){
#ifdef __WIN32
    tray_icon->showMessage(title,msg);
#endif
}


#undef NEED_NO_TOP



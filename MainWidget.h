#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <ClipBoardContent.h>
#include <Data.h>
#include <QQueue>
#include <QListView>
#include <QStandardItemModel>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QSystemTrayIcon>
#include <QMenu>
#include <AutoHide.h>


class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

signals:
    void item_clicked(int);
    void item_need_edit(int);
    void setting_changed();
    void set_data(Data,int);
    void delete_record(int);
    void save_to_file_sgn(QString);
    void load_from_file_sgn(QString);
    void export_image_sgn(QString);
    void export_urls_sgn(QString);
    void export_text_sgn(QString);
    void export_text_one_sgn(QString);
    void export_html_sgn(QString);
    void export_html_one_sgn(QString);
    void clear_all_sgn();


public slots:
    void on_data_changed(QQueue<Data> data);
    void on_item_clicked(QModelIndex index);
    void on_setting_changed();
    void on_filter_btn_clicked();
    void on_edit_data(Data data,int index);
    void on_edit_btn_clicked();
    void on_delete_record();
    void on_set_auto_save_file();
    void on_save_to_file();
    void on_load_from_file();
    void on_export_image();
    void on_export_urls();
    void on_export_text();
    void on_export_text_single();
    void on_export_html();
    void on_export_html_single();
    void on_show_hide_widget();
    void on_real_exit();
    void on_customContextMenuRequested();
    void on_about();
    void on_clear_all();
    void on_auto_hide();

private:
    ClipBoardContent *pClipContent;
    QListView *pView;
    QStandardItemModel *pModel;
    QCheckBox *enable_watch;
    QPushButton *filter_btn;
    QPushButton *edit_btn;
    QPushButton *delete_record_btn;

    QCheckBox *enable_auto_save;
    QLabel *auto_save_file_tip;
    QLineEdit *auto_save_file_name;
    QPushButton *auto_save_file_name_btn;

    QLabel *record_count_tip;
    QLineEdit *record_count_edit;
    QPushButton *record_count_set_btn;

    QPushButton *save_to_file;
    QPushButton *load_from_file;
    QPushButton *export_image;
    QPushButton *export_urls;


    QPushButton *export_text;
    QPushButton *export_text_one;
    QPushButton *export_html;
    QPushButton *export_html_one;

    QPushButton *about_btn;
    QPushButton *clear_all;
    QPushButton *rel_close;
    QPushButton *hide_btn;

    AutoHide *auto_hide_widget;

    bool real_exit;
    bool auto_hide;


    QSystemTrayIcon *tray_icon;

    QMenu *tray_menu;

    QString get_abstract(const Data &data)const;
    void read_setting();

    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void flush_settings();
};

#endif // MAINWIDGET_H

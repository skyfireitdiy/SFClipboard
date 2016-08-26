#ifndef DATAEDITWND_H
#define DATAEDITWND_H
#include <QDialog>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>
#include <QStandardItemModel>
#include <Data.h>

class DataEditWnd : public QDialog
{
    Q_OBJECT
public:
    Data data;
    DataEditWnd(Data data_t, QWidget *parent=0);
private:

    QGroupBox *text_group;
    QPlainTextEdit *text_edit;
    QGroupBox *html_group;
    QGroupBox *html_edit_group;
    QGroupBox *html_show_group;
    QPlainTextEdit *html_edit;
    QTextBrowser *html_show;
    QGroupBox *image_group;
    QLabel *image_show;
    QLabel *image_path_tip;
    QLineEdit *image_path;
    QPushButton *get_image_path_btn;
    QPushButton *image_clear_btn;
    QGroupBox *urls_group;
    QListView *urls_list;
    QStandardItemModel *urls_model;
    QPushButton *urls_del_btn;
    QLineEdit *urls_edit;
    QPushButton *urls_add_btn;
    QGroupBox *color_group;
    QLabel *red_label;
    QLabel *green_label;
    QLabel *blue_label;
    QLineEdit *red_edit;
    QLineEdit *green_edit;
    QLineEdit *blue_edit;
    QLabel *color_show;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;


    bool color_changed;

public slots:
    void on_color_changed(QColor color);
    void on_color_value_changed();
    void on_html_chnaged();
    void on_image_path_btn_clicked();
    void on_del_urls();
    void on_add_urls();
    void on_ok();
    void on_cancel();
    void on_image_clear();
};

#endif // DATAEDITWND_H

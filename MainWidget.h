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
public slots:
    void on_data_changed(QQueue<Data> data);
    void on_item_clicked(QModelIndex index);
    void on_setting_changed();
    void on_filter_btn_clicked();
    void on_edit_data(Data data,int index);
    void on_edit_btn_clicked();
    void on_delete_record();
    void on_set_auto_save_file();
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

    QString get_abstract(const Data &data)const;
    void read_setting();
};

#endif // MAINWIDGET_H

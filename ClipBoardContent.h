#ifndef CLIPBOARDCONTENT_H
#define CLIPBOARDCONTENT_H

#include <QObject>
#include <QClipboard>
#include <QQueue>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <Data.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <SFPassword.h>


class ClipBoardContent : public QObject
{
    Q_OBJECT
public:
    explicit ClipBoardContent(QObject *parent = 0);

signals:
    void data_changed(QQueue<Data>);
    void edit_data(Data,int);
public slots:
    void on_clip_data_changed();
    void read_setting();
    void flush_record();
    void on_set_clip_data(int index);
    void on_data_edit(int index);
    void on_set_data(Data dt,int index);
    void on_delete_record(int index);
    void on_save_to_file(QString file_name);
    void on_load_from_file(QString file_name);
    void on_export_image(QString dir);
    void on_export_urls(QString file_name);
    void on_export_text(QString dir);
    void on_export_text_single(QString file_name);
    void on_export_html(QString dir);
    void on_export_html_single(QString file_name);
    void on_clear_all();

private:
    QQueue<Data> data={};
    QClipboard *pClip={nullptr};
    int data_type={0};
    int max_record_count={0};
    bool selecting={false};
    bool editing={false};
    bool enable={false};
    bool is_auto_save={false};
    QString auto_save_file={""};

    QSqlDatabase database=QSqlDatabase::addDatabase("QSQLITE",QLatin1String("SFClipboard_DB"));
    QString database_file_name={"SFC.db"};
    QString database_user={"SkyFire"};
    SFPassword database_ps{QByteArray("SkyFire"),(QObject*)(this)};

    void save_to_file(Data dt);
};

#endif // CLIPBOARDCONTENT_H

#ifndef TYPEFILTER_H
#define TYPEFILTER_H
#include <QDialog>
#include <QCheckBox>
#include <QPushButton>

class TypeFilter : public QDialog
{
    Q_OBJECT
public:
    TypeFilter(QWidget *parent=0);
    QCheckBox *enable_text;
    QCheckBox *enable_html;
    QCheckBox *enable_url;
    QCheckBox *enable_image;
    QCheckBox *enable_color;
    QPushButton *ok_btn;
    QPushButton *cancle_btn;
public slots:
    void on_ok_btn_clicked();
    void on_cancle_btn_clicked();
};

#endif // TYPEFILTER_H

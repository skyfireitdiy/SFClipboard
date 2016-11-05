#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QVariant>
#include <QUrl>
#include <QDateTime>

class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QObject *parent = 0);
    Data(const Data& other, QObject *parent=0);
    const Data& operator =(const Data& other);
    bool operator ==(const Data& other)const;
    bool operator !=(const Data& other)const;
    int type;
    QString text;
    QString html;
    QVariant image;
    QVariant color;
    QList<QUrl> urls;

    void clear();
signals:

public slots:
};

#endif // DATA_H

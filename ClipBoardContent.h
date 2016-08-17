#ifndef CLIPBOARDCONTENT_H
#define CLIPBOARDCONTENT_H

#include <QObject>

class ClipBoardContent : public QObject
{
    Q_OBJECT
public:
    explicit ClipBoardContent(QObject *parent = 0);

signals:

public slots:
};

#endif // CLIPBOARDCONTENT_H
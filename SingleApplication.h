#ifndef MYSINGLEAPPLICATION_H
#define MYSINGLEAPPLICATION_H
#include <QApplication>
#include <QSharedMemory>
class SingleApplication : public QObject{
    Q_OBJECT
public:
    SingleApplication( const QString uniqueKey);
    ~SingleApplication();
    bool isRunning();
    void detach();
private:
    bool _isRunning;
    QString _uniqueKey;
    QSharedMemory sharedMemory;
};

#endif // MYSINGLEAPPLIACTION_H

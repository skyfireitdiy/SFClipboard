#ifndef MYSINGLEAPPLICATION_H
#define MYSINGLEAPPLICATION_H
#include <QApplication>
#include <QSharedMemory>
class SingleApplication : public QApplication{
    Q_OBJECT
public:
    SingleApplication(int &argc, char *argv[], const QString uniqueKey);
    bool isRunning();
private:
    bool _isRunning;
    QString _uniqueKey;
    QSharedMemory sharedMemory;
};

#endif // MYSINGLEAPPLIACTION_H

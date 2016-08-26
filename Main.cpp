#include <QApplication>
#include <MainWidget.h>
#include <ClipBoardContent.h>
#include <Global.h>

QApplication *pApp=0;
QSettings *pSettings;

int main(int argc,char ** argv){
    QApplication app(argc,argv);
    qRegisterMetaType<QQueue<Data>>("QQueue<Data>");
    pApp=&app;
    pSettings=new QSettings("sfclip.ini",QSettings::IniFormat,pApp);
    MainWidget w;
    w.show();
    return app.exec();
}

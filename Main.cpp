#include <QApplication>
#include <MainWidget.h>
#include <ClipBoardContent.h>
#include <Global.h>

QApplication *pApp=0;
QSettings *pSettings;
QString version="1.1";
MainWidget *pMainWidget=nullptr;

int main(int argc,char ** argv){
    QApplication app(argc,argv);
    qRegisterMetaType<QQueue<Data>>("QQueue<Data>");
    pApp=&app;
    pSettings=new QSettings("sfclip.ini",QSettings::IniFormat,pApp);
    MainWidget w;
    pMainWidget=&w;
    w.show();
    return app.exec();
}

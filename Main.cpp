#include <QApplication>
#include <MainWidget.h>
#include <ClipBoardContent.h>
#include <Global.h>
#include <QMessageBox>
#include <SingleApplication.h>
#include <QDebug>

EXTERN_SF_LAN

SingleApplication *pApp=0;
QSettings *pSettings;
QString version="2.3";
MainWidget *pMainWidget=nullptr;
QString programName="SFClipboard";

int main(int argc,char ** argv){
    SingleApplication app(argc,argv,programName);
    pSettings=new QSettings("sfclip.ini",QSettings::IniFormat,pApp);
    SET_LANG(pSettings->value("lang").toString());
    if(app.isRunning()){
        QMessageBox::information(nullptr,GS("MESSAGE"),GS("STILL_RUN"),QMessageBox::Ok);
        return 0;
    }
    qRegisterMetaType<QQueue<Data>>("QQueue<Data>");
    pApp=&app;
    MainWidget w;
    pMainWidget=&w;
    w.show();
    int ret=app.exec();
    app.detach();
    return ret;
}

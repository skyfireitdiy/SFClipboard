#include <QApplication>
#include <MainWidget.h>
#include <ClipBoardContent.h>
#include <Global.h>
#include <QMessageBox>
#include <SingleApplication.h>

EXTERN_SF_LAN

QApplication *pApp=0;
QSettings *pSettings;
QString version="1.9";
MainWidget *pMainWidget=nullptr;
QString programName="SFClipboard";

int main(int argc,char ** argv){
    SingleApplication app(argc,argv,programName);
    if(app.isRunning()){
        QMessageBox::information(nullptr,GS(MESSAGE),GS(STILL_RUN),QMessageBox::Ok);
        return 0;
    }
    qRegisterMetaType<QQueue<Data>>("QQueue<Data>");
    pApp=&app;
    pSettings=new QSettings("sfclip.ini",QSettings::IniFormat,pApp);
    MainWidget w;
    pMainWidget=&w;
    w.show();
    return app.exec();
}

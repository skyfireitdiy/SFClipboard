#include <QApplication>
#include <MainWidget.h>
#include <ClipBoardContent.h>
#include <Global.h>
#include <QMessageBox>
#include <SingleApplication.h>

QApplication *pApp=0;
QSettings *pSettings;
QString version="1.7";
MainWidget *pMainWidget=nullptr;
QString programName="SFClipboard";

int main(int argc,char ** argv){
    SingleApplication app(argc,argv,programName);
    if(app.isRunning()){
        QMessageBox::information(nullptr,"消息","SFClipboard已在运行，请见托盘图标",QMessageBox::Ok);
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

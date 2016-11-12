#include <QApplication>
#include <MainWidget.h>
#include <ClipBoardContent.h>
#include <Global.h>
#include <QMessageBox>
#include <SingleApplication.h>
#include <QDebug>
#ifdef __WIN32
#include <SFReg.h>
#endif

EXTERN_SF_LAN

SingleApplication *pApp=0;
QSettings *pSettings;
QString version="2.5";
MainWidget *pMainWidget=nullptr;
QString programName="SFClipboard";

int main(int argc,char ** argv){
    SingleApplication app(argc,argv,programName);
    pSettings=new QSettings("sfclip.ini",QSettings::IniFormat,pApp);
    SET_LANG(pSettings->value("lang").toString());

#ifdef __WIN32
    if(!IsRunasAdmin()){
        app.detach();
        QString param="/adminoption 1";
        SHELLEXECUTEINFOA execinfo;
        memset(&execinfo, 0, sizeof(execinfo));
        execinfo.lpFile         = argv[0];
        execinfo.cbSize         = sizeof(execinfo);
        execinfo.lpVerb         = "runas";
        execinfo.fMask          = SEE_MASK_NO_CONSOLE;
        execinfo.nShow          = SW_SHOWDEFAULT;
        execinfo.lpParameters   = param.toLocal8Bit().data();
        ShellExecuteExA(&execinfo);
        return 0;
    }
#endif

    if(app.isRunning()){
        QMessageBox::information(nullptr,GS("MESSAGE"),GS("STILL_RUN"),QMessageBox::Ok);
        return 0;
    }
#ifdef __WIN32
    SFReg::add_win32_auto_run_once(programName,argv[0]);
#endif
    qRegisterMetaType<QQueue<Data>>("QQueue<Data>");
    pApp=&app;
    MainWidget w;
    pMainWidget=&w;
    w.show();
    int ret=app.exec();
    app.detach();
    return ret;
}

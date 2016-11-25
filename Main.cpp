#include <QApplication>
#include <MainWidget.h>
#include <ClipBoardContent.h>
#include <Global.h>
#include <QMessageBox>
#include <SingleApplication.h>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#ifdef __WIN32
#include <SFReg.h>
#include <versionhelpers.h>
#endif

EXTERN_SF_LAN

SingleApplication *pApp=0;
QSettings *pSettings;
QString version="3.0";
MainWidget *pMainWidget=nullptr;
QString programName="SFClipboard";

int main(int argc,char ** argv){
    SingleApplication app(argc,argv,programName);
    pApp=&app;
#ifdef __WIN32
    char file_full_path[256]{0};
    GetModuleFileNameA(0,file_full_path,256);
    QFileInfo curr_file(file_full_path);
    SetCurrentDirectoryA(curr_file.path().toLocal8Bit());
#endif
    pSettings=new QSettings("sfclip.ini",QSettings::IniFormat,pApp);
    SET_LANG(pSettings->value("lang").toString());

#ifdef __WIN32
    if(argc==1){
        if(IsWindowsVistaOrGreater()){
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
        }else{
            pApp->detach();
            QProcess process;
            process.startDetached(argv[0]+QString(" /low"));
            return 0;
        }
    }else if(argc==2){
        if(argv[1]==QString("/low")){

        }else{
            QMessageBox::critical(0,GS("WARNING"),GS("PARAM_ERR_TIP"));
            pApp->detach();
            return 0;
        }
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
    MainWidget w;
    pMainWidget=&w;
    w.show();
    int ret=app.exec();
    app.detach();
    return ret;
}

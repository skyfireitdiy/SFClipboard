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
#include <Windows.h>
#else

#endif

#include <GetPs.h>
#include <SFPassword.h>

EXTERN_SF_LAN

SingleApplication *pApp=0;
QSettings *pSettings;
QString version="3.3.0.9";
MainWidget *pMainWidget=nullptr;
QString programName="SFClipboard";

int main(int argc,char ** argv){
    SingleApplication app(argc,argv,programName);
    pApp=&app;
    pSettings=new QSettings("sfclip.ini",QSettings::IniFormat,pApp);
#ifdef __WIN32
    char file_full_path[256]{0};
    GetModuleFileNameA(0,file_full_path,256);
    QFileInfo curr_file(file_full_path);
    SetCurrentDirectoryA(curr_file.path().toLocal8Bit());
#endif
    QFile lang_file("lang.ini");
    if(pSettings->value("version").toString()!=version||!lang_file.exists()){
        QFile lang_rc_file(":/str/bin/lang.ini");
        remove("lang.ini");
        lang_rc_file.copy("lang.ini");
        pSettings->setValue("lang","Chinese");
        pSettings->sync();
    }
    __lan_st=new SFLanguage("lang.ini","Chinese");

    QString lang_type=pSettings->value("lang").toString();
    lang_type=lang_type.isEmpty()?"Chinese":lang_type;
    __lan_st=new SFLanguage("lang.ini",lang_type);
    SET_LANG(lang_type);
#ifdef __WIN32
    if(argc==1){
        OSVERSIONINFO   osver;
        osver.dwOSVersionInfoSize   =   sizeof(OSVERSIONINFO);
        GetVersionEx(&osver);
        if(osver.dwPlatformId == 2)
        {
            if(osver.dwMajorVersion >=  6){
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
                    exit(0);
                }
            }else{
                pApp->detach();
                QProcess process;
                process.startDetached(argv[0]+QString(" /low"));
                exit(0);
            }
        }
    }else if(argc==2){
        if(argv[1]==QString("/low")){

        }else{
            QMessageBox::critical(0,GS("WARNING"),GS("PARAM_ERR_TIP"));
            pApp->detach();
            exit(0);
        }
    }

#else
    if(!if_is_linux_root()){
        GetPs pswnd(0);
        pswnd.exec();
        app.detach();
        SFPassword ps(pswnd.get_ps());
        QFile file(":/shell/resource/run_as_root.sh");
        file.copy("run_as_root.sh");
        run_process("chmod +x run_as_root.sh");
        qDebug()<<"./run_as_root.sh "+QString::fromLocal8Bit(ps)+" "+qApp->arguments()[0];
        QProcess::startDetached("./run_as_root.sh "+QString::fromLocal8Bit(ps)+" "+qApp->arguments()[0]);
        exit(0);
    }

#endif
    if(app.isRunning()){
        QMessageBox::information(nullptr,GS("MESSAGE"),GS("STILL_RUN"),QMessageBox::Ok);
        exit(0);
    }
    qRegisterMetaType<QQueue<Data>>("QQueue<Data>");
    MainWidget w;
    pMainWidget=&w;
    w.show();
    int ret=app.exec();
    app.detach();
    return ret;
}

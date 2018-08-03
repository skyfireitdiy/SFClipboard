#include <Global.h>
#include <QProcess>
#include <QFile>
#include <QDebug>

#ifdef __WIN32

BOOL IsRunasAdmin()
{
    BOOL bElevated = FALSE;
    HANDLE hToken = NULL;
    if ( !OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken ) )
        return FALSE;

    TOKEN_ELEVATION tokenEle;
    DWORD dwRetLen = 0;

    // Retrieve token elevation information
    if ( GetTokenInformation( hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen ) )
    {
        if ( dwRetLen == sizeof(tokenEle) )
        {
            bElevated = tokenEle.TokenIsElevated;
        }
    }
    CloseHandle( hToken );
    return bElevated;
}



#else


QString run_process(QString cmd){
    QProcess pro;
    QEventLoop event;
    QString ret_str;
    QObject::connect(&pro,SIGNAL(finished(int)),&event,SLOT(quit()));
    QObject::connect(&pro,&QProcess::readyReadStandardOutput,[&](){ret_str+=pro.readAllStandardOutput();});
    pro.start(cmd);
    event.exec();
    return ret_str;
}


bool if_is_linux_root(){
    qDebug()<<"copy script";
    QFile file(":/shell/resource/is_root.sh");
    file.copy("is_root.sh");
    qDebug()<<"run is_root.sh";
    int ret=run_process("bash ./is_root.sh").toInt();
    qDebug()<<"root result"<< ret;
    return ret;
}


#endif

#include <QDebug>

bool set_button_backimg(QPushButton *button, QString img_path){
    QPixmap pixmap(img_path);
    if(pixmap.isNull())
        return false;
    if(button==nullptr)
        return false;
    button->setFixedSize(pixmap.size());
    button->setMask(pixmap.createHeuristicMask());
    button->setIcon(pixmap);
    button->setIconSize(pixmap.size());
    button->update();
    return true;
}

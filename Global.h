#ifndef GLOBAL_H
#define GLOBAL_H
#include <QApplication>
#include <QSettings>
#include <SFLanguage.h>
#include <SingleApplication.h>
#include <QPushButton>
#include <QPixmap>
#include <QBitmap>
#ifdef __WIN32
#include <windows.h>

#endif

extern int  TEXT ;
extern int  HTML ;
extern int  IMAGE ;
extern int  COLOR ;
extern int  URLS ;

extern SingleApplication *pApp;
extern QSettings *pSettings;
extern QString version;

#ifdef __WIN32
BOOL IsRunasAdmin();

#else

bool if_is_linux_root();
QString run_process(QString cmd);


#endif

bool set_button_backimg(QPushButton *button, QString img_path);

#endif // GLOBAL_H

#ifndef GLOBAL_H
#define GLOBAL_H
#include <QApplication>
#include <QSettings>
#include <SFLanguage.h>
#include <SingleApplication.h>
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

#endif



#endif // GLOBAL_H

QT+=gui widgets sql

HEADERS += \
    MainWidget.h \
    Global.h \
    ClipBoardContent.h \
    Data.h \
    TypeFilter.h \
    DataEditWnd.h \
    AutoHide.h \
    WinHook.h \
    SingleApplication.h \
    SFLanguage.h \
    LinuxHook.h \
    SFAES.h \
    SFPassword.h


SOURCES += \
    MainWidget.cpp \
    Main.cpp \
    ClipBoardContent.cpp \
    Data.cpp \
    TypeFilter.cpp \
    DataEditWnd.cpp \
    AutoHide.cpp \
    WinHook.cpp \
    SingleApplication.cpp \
    SFLanguage.cpp \
    LinuxHook.cpp \
    Global.cpp \
    SFAES.cpp \
    SFPassword.cpp

win32{
HEADERS +=SFReg.h
SOURCES += SFReg.cpp
}

CONFIG+= c++11

DISTFILES += \
    readme.md \
    icon.rc \
    version.md \
    install/information_after.txt \
    install/information_before.txt \
    install/install.iss \
    install/licence.txt \
    windows.bat \
    install/icon.ico \
    bin/lang.ini \
    bin/SFClipboard.exe

RESOURCES += \
    resource.qrc

RC_FILE+=icon.rc

OBJECTS_DIR = $$PWD/obj
MOC_DIR= $$PWD/moc
UI_DIR= $$PWD/ui
DESTDIR = $$PWD/bin


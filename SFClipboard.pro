QT+=gui widgets

HEADERS += \
    MainWidget.h \
    Global.h \
    ClipBoardContent.h \
    Data.h \
    TypeFilter.h \
    DataEditWnd.h \
    AutoHide.h \
    WinHook.h \
    SingleApplication.h

SOURCES += \
    MainWidget.cpp \
    Main.cpp \
    ClipBoardContent.cpp \
    Data.cpp \
    TypeFilter.cpp \
    DataEditWnd.cpp \
    AutoHide.cpp \
    WinHook.cpp \
    SingleApplication.cpp

CONFIG+= c++11

DISTFILES += \
    readme.md \
    icon.rc \
    version.md \
    install/information_after.txt \
    install/information_before.txt \
    install/install.iss \
    install/licence.txt

RESOURCES += \
    resource.qrc

RC_FILE+=icon.rc

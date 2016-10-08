QT+=gui widgets

HEADERS += \
    MainWidget.h \
    Global.h \
    ClipBoardContent.h \
    Data.h \
    TypeFilter.h \
    DataEditWnd.h \
    AutoHide.h \
    WinHook.h

SOURCES += \
    MainWidget.cpp \
    Main.cpp \
    ClipBoardContent.cpp \
    Data.cpp \
    TypeFilter.cpp \
    DataEditWnd.cpp \
    AutoHide.cpp \
    WinHook.cpp

CONFIG+= c++11

DISTFILES += \
    readme.md \
    icon.rc

RESOURCES += \
    resource.qrc

RC_FILE+=icon.rc

QT+=gui widgets

HEADERS += \
    MainWidget.h \
    Global.h \
    ClipBoardContent.h \
    Data.h \
    TypeFilter.h \
    DataEditWnd.h

SOURCES += \
    MainWidget.cpp \
    Main.cpp \
    ClipBoardContent.cpp \
    Data.cpp \
    TypeFilter.cpp \
    DataEditWnd.cpp

CONFIG+= c++11

DISTFILES += \
    readme.md

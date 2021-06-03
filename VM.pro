#-------------------------------------------------
#
# Project created by QtCreator 
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VM
TEMPLATE = app
#zly for minidump
#加入调试信息
QMAKE_CFLAGS_RELEASE += -g
QMAKE_CXXFLAGS_RELEASE += -g
#禁止优化
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O2
#release在最后link时默认有"-s”参数，表示"Omit all symbol information from the output file"，因此要去掉该参
win32:!msvc{
    QMAKE_LFLAGS_RELEASE = -mthreads
}else{
    #message("msvc version")
    #QMAKE_CFLAGS_RELEASE = -O2 -MD -Zi
    #QMAKE_LFLAGS_RELEASE = /INCREMENTAL:NO /DEBUG
    #QMAKE_LFLAGS_RELEASE += /MAP
    #QMAKE_CFLAGS_RELEASE += /Zi
    #QMAKE_LFLAGS_RELEASE += /debug /opt:ref
    QMAKE_LFLAGS_RELEASE += /MAP /DEBUG /opt:ref /INCREMENTAL:NO
}
#

SOURCES += \
    main.cpp \
    iconhelper.cpp \
    mpvwidget.cpp \
    myapp.cpp \
    qvideowidget.cpp \
    switchbutton.cpp \
    excelhelper.cpp \
    frmmessagebox.cpp \
    tlable.cpp \
    treewidget.cpp \
    frmmain.cpp \
    frminputbox.cpp \    
    frmconfig.cpp \    
    frmnvr.cpp \
    frmipc.cpp \
    frmpollconfig.cpp

HEADERS  += \
    mpvwidget.h \
    myhelper.h \
    iconhelper.h \
    myapp.h \
    qvideowidget.h \
    switchbutton.h \
    excelhelper.h \
    frmmessagebox.h \
    frmmain.h \
    frminputbox.h \
    tlable.h \
    treewidget.h \
    frmconfig.h \    
    frmnvr.h \
    frmipc.h \
    frmpollconfig.h

FORMS    += \
    frmmessagebox.ui \
    frmmain.ui \
    frminputbox.ui \
    frmconfig.ui \
    frmnvr.ui \
    frmipc.ui \
    frmpollconfig.ui

INCLUDEPATH 	+= $$PWD
INCLUDEPATH += $$PWD/libmpv
LIBS += $$PWD/libmpv/libmpv.dll.a
#zly
LIBS += \
-lUser32 \
-lDbghelp
#zly


MOC_DIR         = temp/moc
RCC_DIR         = temp/rcc
UI_DIR          = temp/ui
OBJECTS_DIR     = temp/obj
DESTDIR         = bin

win32:RC_FILE=main.rc

RESOURCES += \
    rc.qrc

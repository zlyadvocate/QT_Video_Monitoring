#-------------------------------------------------
#
# Project created by QtCreator 
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VM
TEMPLATE = app

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


MOC_DIR         = temp/moc
RCC_DIR         = temp/rcc
UI_DIR          = temp/ui
OBJECTS_DIR     = temp/obj
DESTDIR         = bin

win32:RC_FILE=main.rc

RESOURCES += \
    rc.qrc

#-------------------------------------------------
#
# Project created by QtCreator 2015-12-14T18:43:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DSTDedicatedServerGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ioworldgenoverridelua.cpp \
    iomodoverrideslua.cpp \
    qscrolltext.cpp

HEADERS  += mainwindow.h \
    ioworldgenoverridelua.h \
    dstdatastructure.h \
    iomodoverrideslua.h \
    qscrolltext.h

FORMS    += mainwindow.ui

TRANSLATIONS += translations/dstdsgui_zh_HK.ts

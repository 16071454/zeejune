#-------------------------------------------------
#
# Project created by QtCreator 2019-04-15T14:08:46
#
#-------------------------------------------------

QT       += widgets
QT       += core gui

include($(MTPDIR)/source/pri/guiapk.pri)
INCLUDEPATH	+= $(MTPDIR)/source/include/guiapk/$$TARGET .
INCLUDEPATH += $(MTPDIR)/source/src/guiapk .
INCLUDEPATH += $(MTPDIR)/source/include/thirdpartyinc/kdchart .
INCLUDEPATH += $(MTPDIR)/source/include/historyapk .
INCLUDEPATH += $(MTPDIR)/source/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mtcurve
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mtlistthread.cpp \
    mtkdchart.cpp\
   TableModel.cpp \
    datatabledialog.cpp \ 
    mylinediagram.cpp \
    chartpaintthread.cpp \
    mtbasecurvedialog.cpp \
    qupdatethread.cpp

HEADERS  += mainwindow.h \
    mtlistthread.h \
    mtkdchart.h\
   TableModel.h \
    datatabledialog.h \
    mylinediagram.h \
    chartpaintthread.h \
    mtbasecurvedialog.h \
    qupdatethread.h

FORMS    += mainwindow.ui \
    datatabledialog.ui \
    mtbasecurvedialog.ui

LIBS += -lhiscurve -lhisdbaccess -lkdchartd2
LIBS += -lcommonlib
LIBS += -lguilib
LIBS += -lmtobjsyslib 
LIBS += -lmodelimportoollib
LIBS += -lnetcore
LIBS += -lscadamgr
LIBS += -lfesinfoproclib
LIBS += -lscadaapi
LIBS += -lscadaserverlib
LIBS += -lrtdbcore
LIBS += -lguibaselib
LIBS += -lmtscadaobjlib


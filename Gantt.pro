#-------------------------------------------------
#
# Project created by QtCreator 2016-08-10T12:01:19
#
#-------------------------------------------------

QT       += core gui printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gantt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ganttrect.cpp \
    ganttflow.cpp \
    qtablewidgetnumberitem.cpp \
    gclgms.c \
    gdxcc.c \
    wizard.cpp \
    exportwizard.cpp


HEADERS  += mainwindow.h \
    ganttrect.h \
    ganttflow.h \
    constants.h \
    qtablewidgetnumberitem.h \
    gclgms.h \
    gdxcc.h \
    wizard.h \
    exportwizard.h

FORMS    += mainwindow.ui \
    wizard.ui \
    exportwizard.ui

DISTFILES += \
    gantticon.png \
    gantticon.ico \
    icon.icns \
    gantt.rc

RC_FILE = gantt.rc
ICON = icon.icns

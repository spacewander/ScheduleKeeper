#-------------------------------------------------
#
# Project created by QtCreator 2014-11-24T20:57:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScheduleKeeper
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    settingsdialog.cpp \
    editjournalpanel.cpp \
    localjournal.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    global.h \
    settingsdialog.h \
    editjournalpanel.h \
    localjournal.h

FORMS    +=

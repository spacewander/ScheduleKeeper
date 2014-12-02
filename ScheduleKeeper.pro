#-------------------------------------------------
#
# Project created by QtCreator 2014-11-24T20:57:09
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScheduleKeeper
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    settingsdialog.cpp \
    editjournalpanel.cpp \
    localjournal.cpp \
    journallistmodel.cpp \
    userstable.cpp \
    journalstable.cpp \
    net.cpp \
    basicjournal.cpp \
    detailjournal.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    global.h \
    settingsdialog.h \
    editjournalpanel.h \
    localjournal.h \
    journallistmodel.h \
    userstable.h \
    journalstable.h \
    net.h \
    basicjournal.h \
    detailjournal.h \
    remotejournal.h

FORMS    +=

RESOURCES += \
    ScheduleKeeper.qrc

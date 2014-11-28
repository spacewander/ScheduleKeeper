#include <QDebug>

#include "localjournal.h"

void logDeleteLocalJournal(const LocalJournal& journal)
{
    qDebug() << "delete localjournal: journalID " << journal.journalID;
    if (!journal.isDeleted) {
        qDebug() << "Error: isDeleted has not been set to true";
    }
}

void logSaveLocalJournal(const LocalJournal& journal)
{
    qDebug() << "save localjournal: journalID " << journal.journalID;
    qDebug() << "createdTime: " << journal.createdTime;
    qDebug() << "saveTime: " << journal.saveTime;
    if (journal.willAlarm) {
        qDebug() << "alarmTime: " << journal.alarmTime;
    }
}

void logCreateLocalJournal(const LocalJournal& journal)
{
    qDebug() << "create localjournal: journalID " << journal.journalID;
    qDebug() << "createdTime: " << journal.createdTime;
    qDebug() << "saveTime: " << journal.saveTime;
    if (journal.willAlarm) {
        qDebug() << "alarmTime: " << journal.alarmTime;
    }
}

LocalJournal::LocalJournal()
{
    isDeleted = true;
    journalID = "";
}

LocalJournal::LocalJournal(const QString& journalID, const QDateTime& ctime,
             const QDateTime& mtime, const QString& detail)
    : detail(detail), journalID(journalID), saveTime(mtime), 
    createdTime(ctime)
{
}

LocalJournal::LocalJournal(const QString& journalID, const QDateTime& ctime,
             const QDateTime& mtime, const QString& detail,
             const QDateTime& alarmTime)
    : LocalJournal(journalID, ctime, mtime, detail)
{
    willAlarm = true;
    this->alarmTime = alarmTime;
}

void LocalJournal::clear()
{
    isDeleted = true;
    detail = "";
    willAlarm = false;
    alarmTime = QDateTime();
    saveTime = QDateTime();
    createdTime = QDateTime();
    journalID = "";
}


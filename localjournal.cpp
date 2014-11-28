#include <QDebug>

#include "localjournal.h"

const int detailDisplayLength = 70;

void logDeleteLocalJournal(const LocalJournal& journal)
{
    qDebug() << "delete localjournal: journalID " << journal.journalID;
    if (!journal.deleted) {
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
    deleted = true;
    willAlarm = false;
    journalID = "";
}

LocalJournal::LocalJournal(const QString& journalID, const QDateTime& ctime,
             const QDateTime& mtime, const QString& detail)
    : detail(detail), journalID(journalID), saveTime(mtime), 
    createdTime(ctime)
{
    willAlarm = false;
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
    deleted = true;
    detail = "";
    willAlarm = false;
    alarmTime = QDateTime();
    saveTime = QDateTime();
    createdTime = QDateTime();
    journalID = "";
}

void LocalJournal::generalizeID()
{
    QDateTime current = QDateTime::currentDateTimeUtc();
    journalID = QString::number(
                QDateTime(QDate(1970, 1, 1), QTime(0, 0)).secsTo(current));
}

const QString LocalJournal::getAlarm() const
{
    return alarmTime.toString("yy年MM月dd日hh：mm");
}

const QString LocalJournal::getDetail() const
{
    if (detail.size() > detailDisplayLength) {
        return detail.left(detailDisplayLength) + "...";
    }
    return detail;
}


#include <QDebug>

#include "localjournal.h"

const int detailDisplayLength = 70;

void logDeleteLocalJournal(const LocalJournal& journal)
{
    qDebug() << "delete localjournal: journalId " << journal.journalId;
    if (!journal.deleted) {
        qDebug() << "Error: isDeleted has not been set to true";
    }
}

void logSaveLocalJournal(const LocalJournal& journal)
{
    qDebug() << "save localjournal: journalId " << journal.journalId;
    qDebug() << "saveTime: " << journal.saveTime;
    if (journal.willAlarm) {
        qDebug() << "alarmTime: " << journal.alarmTime;
    }
}

void logCreateLocalJournal(const LocalJournal& journal)
{
    qDebug() << "create localjournal: journalId " << journal.journalId;
    qDebug() << "saveTime: " << journal.saveTime;
    if (journal.willAlarm) {
        qDebug() << "alarmTime: " << journal.alarmTime;
    }
}

bool operator <(const LocalJournal& a, const LocalJournal& b)
{
    return (a.journalId < b.journalId);
}

LocalJournal::LocalJournal()
{
    deleted = true;
    willAlarm = false;
    journalId = "";
}

LocalJournal::LocalJournal(const QString& journalId,
             const QDateTime& mtime, const QString& detail)
    : detail(detail), journalId(journalId), saveTime(mtime)
{
    willAlarm = false;
    deleted = false;
}

LocalJournal::LocalJournal(const QString& journalId,
             const QDateTime& mtime, const QString& detail,
             const QDateTime& alarmTime)
    : LocalJournal(journalId, mtime, detail)
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
    journalId = "";
}

void LocalJournal::generalizeID()
{
    QDateTime current = QDateTime::currentDateTimeUtc();
    journalId = QString::number(current.toMSecsSinceEpoch());
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


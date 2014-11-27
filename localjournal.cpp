#include "localjournal.h"

LocalJournal::LocalJournal()
{
    isDeleted = true;
}

LocalJournal::LocalJournal(const QString& journalID, const QDateTime& ctime,
             const QDateTime& mtime, const QString& detail)
    : journalID(journalID), createdTime(ctime), saveTime(mtime),
      detail(detail)
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

#ifndef LOCALJOURNAL_H
#define LOCALJOURNAL_H

#include <QDateTime>
#include <QString>

/**
 * @brief The LocalJournal class
 * LocalJournal is a POD class just contained necessary data
 */
class LocalJournal
{
public:
    LocalJournal();
    LocalJournal(const QString& journalID, const QDateTime& ctime,
                 const QDateTime& mtime, const QString& detail);
    LocalJournal(const QString& journalID, const QDateTime& ctime,
                 const QDateTime& mtime, const QString& detail,
                 const QDateTime& alarmTime);
    void clear();

    QString  detail;
    QDateTime alarmTime;
    bool willAlarm;
    QString journalID;
    QDateTime saveTime;
    QDateTime createdTime;

    bool isDeleted;
};

#endif // LOCALJOURNAL_H

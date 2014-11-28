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
    // 默认构造函数用于在日程编辑面板中，这样该面板就能保存一个空的日程了
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

void logDeleteLocalJournal(const LocalJournal& journal);
void logSaveLocalJournal(const LocalJournal& journal);
void logCreateLocalJournal(const LocalJournal& journal);

#endif // LOCALJOURNAL_H

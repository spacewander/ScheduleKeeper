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
    LocalJournal(const QString& journalID,
                 const QDateTime& mtime, const QString& detail);
    LocalJournal(const QString& journalID,
                 const QDateTime& mtime, const QString& detail,
                 const QDateTime& alarmTime);
    /**
     * @brief clear
     * 清理LocalJournal的状态到默认构造函数构造的状态
     */
    void clear();
    /**
     * @brief generalizeID
     * 生成的ID为当前时间到1970年1月1日0时0分的秒数，类型为QString
     */
    void generalizeID();
    const QString getAlarm() const;
    const QString getDetail() const;

    QString  detail;
    QDateTime alarmTime;
    bool willAlarm;
    // a timestamp from 1970-01-01 or ""
    QString journalID;
    QDateTime saveTime;

    bool deleted;
    /// userName is initialized whenever this record is created
    QString userName;
};

void logDeleteLocalJournal(const LocalJournal& journal);
void logSaveLocalJournal(const LocalJournal& journal);
void logCreateLocalJournal(const LocalJournal& journal);

#endif // LOCALJOURNAL_H

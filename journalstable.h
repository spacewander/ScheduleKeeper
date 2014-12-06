#ifndef JOURNALSTABLE_H
#define JOURNALSTABLE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

#include "localjournal.h"
#include "global.h"

/**
 * 数据库名 ScheduleKeeper
 * 表名     journal
 * DML: create table journal(
 * detail text,
 * savetime datetime,
 * alarmtime datetime,
 * journalId varchar(12),
 * deleted boolean,
 * userName varchar(40),
 * willalarm boolean
 * )
 * 需要事先建好数据库和表，放在可执行文件的当前目录中一起发布。
 */
class JournalsTable : public QObject
{
    Q_OBJECT
public:
    static JournalsTable* getJournalsTable();
    /**
     * @brief deleteJournal
     * @param journalId
     * 不是真正地删掉journal，只是把journalId相等的第一个Journal设置为删除模式
     * 把deleted设置为false，然后journalId设置为""
     */
    bool deleteJournal(const QString& journalId);
    bool updateJournal(const LocalJournal& newJournal);
    bool insertJournal(const LocalJournal& journal);
    const QList<LocalJournal>& selectJournal(SortBy sortBy);
    const QList<LocalJournal>& searchJournal(const QString& query);
signals:

public slots:

private:
    explicit JournalsTable(QObject *parent = 0);
    explicit JournalsTable(const JournalsTable&);
    JournalsTable operator =(JournalsTable&);

    /// checked select
    bool select();
    /// checked submitAll
    bool submitAll();
    bool updateJournal_(int row, const LocalJournal &journal);
    LocalJournal combineJournal(int row);
    QSqlDatabase database;
    QSqlRelationalTableModel* journals;
    QList<LocalJournal> totalJournal;
    QList<LocalJournal> relativeJournal;
};

#endif // JOURNALSTABLE_H

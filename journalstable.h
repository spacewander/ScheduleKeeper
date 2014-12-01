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
 * journalID varchar(12),
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
    bool deleteJournal(const QString& journalID);
    bool updateJournal(const LocalJournal& newJournal);
    bool insertJournal(const LocalJournal& journal);
    const QList<LocalJournal>& selectJournal();
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
    QSqlDatabase database;
    QSqlRelationalTableModel* journals;
    QList<LocalJournal> totalJournal;
    QList<LocalJournal> relativeJournal;
};

#endif // JOURNALSTABLE_H

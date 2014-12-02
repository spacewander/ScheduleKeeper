#include <QDebug>
#include <QFile>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

#include "journalstable.h"

JournalsTable *journalTable = nullptr;

JournalsTable* JournalsTable::getJournalsTable()
{
    if (journalTable == nullptr) {
        journalTable = new JournalsTable();
    }
    return journalTable;
}

JournalsTable::JournalsTable(QObject *parent) :
    QObject(parent), database(QSqlDatabase::addDatabase("QSQLITE", "journal"))
{
    QFile db("./ScheduleKeeper.db");
    if (db.exists()) {
        database.setDatabaseName("./ScheduleKeeper.db");
        if (!database.open()) {
            qDebug() << "open db failed: " << database.lastError();
            exit(1);
        }
        journals = new QSqlRelationalTableModel(parent, database);
        journals->setTable("journal");
        journals->setEditStrategy(QSqlTableModel::OnManualSubmit);
    }
    else {
        qDebug() << "db not exist";
        exit(1);
    }
}

bool JournalsTable::deleteJournal(const QString &journalID)
{
    journals->setFilter(QString(" journalID = '%1'").arg(journalID));
    select();
    if (journals->rowCount() == 1) {
//        do not actually delete
//        journals->removeRows(0,1);
        LocalJournal journal;
        journal.journalID = journalID;
        return updateJournal_(0, journal);
    }
    else {
        qDebug() << "delete: journalID " + journalID + " not found!";
        return false;
    }
    return submitAll();
}

bool JournalsTable::updateJournal(const LocalJournal &newJournal)
{
    journals->setFilter(QString(" journalID = '%1'").arg(newJournal.journalID));
    select();
    if (journals->rowCount() == 1) {
        return updateJournal_(0, newJournal);
    }
    else {
        qDebug() << "update: journalID " + newJournal.journalID + " not found!";
        return false;
    }
    return submitAll();
}

bool JournalsTable::insertJournal(const LocalJournal &journal)
{
    int rowCount = journals->rowCount();
    int row = rowCount > 0 ? rowCount : 0;
    journals->insertRows(row, 1);
    return updateJournal_(row, journal);
}

bool JournalsTable::updateJournal_(int row, const LocalJournal &journal)
{
    journals->setData(journals->index(row, 0), journal.detail);
    journals->setData(journals->index(row, 1), journal.saveTime);
    journals->setData(journals->index(row, 2), journal.alarmTime);
    journals->setData(journals->index(row, 3), journal.journalID);
    journals->setData(journals->index(row, 4), journal.deleted);
    journals->setData(journals->index(row, 5), journal.userName);
    journals->setData(journals->index(row, 6), journal.willAlarm);
    return submitAll();
}

bool JournalsTable::select()
{
    if (!journals->select()) {
        qDebug() << "select from db failed: " << journals->lastError();
        return false;
    }
    return true;
}

bool JournalsTable::submitAll()
{
    if (!journals->submitAll()) {
        qDebug() << "submit failed: " << journals->lastError();
        return false;
    }
    else {
        qWarning() << "submit successed";
        return true;
    }
}

const QList<LocalJournal>& JournalsTable::selectJournal(SortBy sortBy)
{
    journals->setFilter(""); // reset Filter
    switch (sortBy) {
    case SortBySaveTime:
        journals->setSort(1, Qt::DescendingOrder);
        break;
    case SortByRemainder:
        journals->setSort(2, Qt::DescendingOrder);
        break;
    default:
        qWarning() << "new sort type given?";
        break;
    }
    select();
    int rowCount = journals->rowCount();
    qWarning() << "select journals: " << rowCount;
    totalJournal.clear();
    for (int i = 0; i < rowCount; ++i) {
        totalJournal.push_back(combineJournal(i));
    }
    return totalJournal;
}

const QList<LocalJournal>& JournalsTable::searchJournal(const QString& query)
{

    QString likeQuery = query;
    likeQuery.replace("\"", "\\\"");
    likeQuery.replace("'", "\'");
    likeQuery.replace("%", "\%");
    likeQuery.replace("\\", "\\\\");
    likeQuery = "%" + likeQuery + "%";
    journals->setFilter(QString(" detail like '%1'").arg(likeQuery));
    qWarning() << "search with " << likeQuery;
    select();
    int rowCount = journals->rowCount();
    relativeJournal.clear();
    qWarning() << "relative journals: " << rowCount;
    for (int i = 0; i < rowCount; ++i) {
        relativeJournal.push_back(combineJournal(i));
    }
    return relativeJournal;
}

LocalJournal JournalsTable::combineJournal(int row)
{
    QString journalID = journals->record(row).value("journalID").toString();
    QString detail = journals->record(row).value("detail").toString();
    QDateTime mtime = journals->record(row).value("savetime").toDateTime();
    QDateTime alarmtime = journals->record(row).value("alarmtime").toDateTime();
    bool deleted = journals->record(row).value("deleted").toBool();
    bool willAlarm = journals->record(row).value("willalarm").toBool();
    QString userName = journals->record(row).value("userName").toString();
    LocalJournal journal(journalID, mtime, detail, alarmtime);
    journal.willAlarm = willAlarm;
    journal.deleted = deleted;
    journal.userName = userName;
    return journal;
}

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

const QList<LocalJournal>& JournalsTable::selectJournal()
{
    return totalJournal;
}

const QList<LocalJournal>& JournalsTable::searchJournal(const QString& query)
{
    return relativeJournal;
}

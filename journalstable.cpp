#include <QDebug>
#include <QFile>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

#include "journalstable.h"
JournalsTable *table = nullptr;

Journal* JournalsTable::getJournalsTable()
{
    if (table == nullptr) {
        table = new JournalsTable();
    }
    return table;
}

JournalsTable::JournalsTable(QObject *parent) :
    QObject(parent)
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
    return true;
}

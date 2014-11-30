#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

#include "userstable.h"

UsersTable *table = nullptr;

UsersTable* UsersTable::getUsesTable()
{
    if (table == nullptr) {
        table = new UsersTable();
    }
    return table;
}

UsersTable::UsersTable(QObject *parent) :
    QObject(parent), database(QSqlDatabase::addDatabase("QSQLITE"))
{
    QFile db("./ScheduleKeeper.db");
    if (db.exists()) {
        database.setDatabaseName("./ScheduleKeeper.db");
        if (!database.open()) {
            qDebug() << "open db failed: " << database.lastError();
            exit(1);
        }
        users = new QSqlRelationalTableModel(parent, database);
        users->setTable("user");
        users->setEditStrategy(QSqlTableModel::OnManualSubmit);
    }
    else {
        qDebug() << "db not exist";
        exit(1);
    }
}

bool UsersTable::checkUserExist(const QString &username)
{
    users->setFilter(QString("userName = '%1'").arg(username));
    if (!users->select()) {
        qDebug() << "select from db failed: " << users->lastError();
    }

    const QString user = users->record(0).value("userName").toString();
    if (user == "") {
        qDebug() << "username " << username << "not exist";
        return false;
    }
    qDebug() << "username " << user << " exist";
    return true;
}

bool UsersTable::checkUserOk(const QString &username, const QString &password)
{
    return false;
}

QPair<QString, QString> UsersTable::find_by_username(const QString &username)
{
    QPair<QString, QString> pass_with_salt;
    return pass_with_salt;
}

bool hasUser(const QString &username)
{
    UsersTable *table = UsersTable::getUsesTable();
    return table->checkUserExist(username);
}


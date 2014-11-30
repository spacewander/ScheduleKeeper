#include <QCryptographicHash>
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
    select();

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
    QPair<QString, QString> pass_with_salt = find_by_username(username);
    qDebug() << pass_with_salt;
    if (encrypt(password, pass_with_salt.second) ==
            QByteArray(pass_with_salt.first.toStdString().c_str())) {
        return true;
    }
    return false;
}

bool UsersTable::insertUser(const QString &username, const QString &password,
                            const QString &salt)
{
    int row = users->rowCount() - 1;
    users->insertRows(row, 1);
    users->setData(users->index(row, 0), username);
    users->setData(users->index(row, 1), password);
    users->setData(users->index(row, 2), salt);
    return submitAll();
}

const QPair<QString, QString> UsersTable::find_by_username(const QString &username)
{
    QPair<QString, QString> pass_with_salt;
    users->setFilter(QString("userName = '%1'").arg(username));
    select();

    pass_with_salt.first = users->record(0).value("password").toString();
    pass_with_salt.second = users->record(0).value("salt").toString();

    return pass_with_salt;
}

const QByteArray UsersTable::encrypt(const QString& pass, const QString& salt)
{
    QByteArray data;
    return QCryptographicHash::hash(data.append(pass).append(salt),
                                    QCryptographicHash::Md5).toHex();
}

bool UsersTable::select()
{
    if (!users->select()) {
        qDebug() << "select from db failed: " << users->lastError();
        return false;
    }
    return true;
}

bool UsersTable::submitAll()
{
    if (!users->submitAll()) {
        qDebug() << "submit failed: " << users->lastError();
        return false;
    }
    return true;
}

bool hasUser(const QString &username)
{
    UsersTable *table = UsersTable::getUsesTable();
    return table->checkUserExist(username);
}

bool hasUserWithPassword(const QString& username, const QString& password)
{
    UsersTable *table = UsersTable::getUsesTable();
    return table->checkUserOk(username, password);
}

bool addNewUser(const QString& username, const QString& password, 
        const QString& salt)
{
    UsersTable *table = UsersTable::getUsesTable();
    return table->insertUser(username, password, salt);
}

#ifndef USERSTABLE_H
#define USERSTABLE_H

#include <QByteArray>
#include <QObject>
#include <QPair>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

/**
 * 数据库名 ScheduleKeeper
 * 表名     user
 * DML: create table user(
 * userName varchar(40),
 * password char(32),
 * salt char(5)
 * )
 *
 * 用户名限制在40个字符（20个字）以内
 * salt是随机生成的5位字符串
 * password是MD5加密输入的密码+salt之后的32位字符串
 *
 * 需要事先建好数据库和表，放在可执行文件的当前目录中一起发布。
 */
class UsersTable : public QObject
{
    Q_OBJECT
public:
    static UsersTable* getUsersTable();
    bool checkUserExist(const QString& username);
    bool checkUserOk(const QString& username, const QString& password);
    bool insertUser(const QString& username, const QString& password,
                    const QString& salt);
    /**
     * @brief encrypt
     * encrypt with user-entered password and generalized salt
     */
    const QByteArray encrypt(const QString& pass, const QString& salt);
signals:

public slots:

private:
    explicit UsersTable(QObject *parent = 0);
    explicit UsersTable(const UsersTable&);
    UsersTable operator =(UsersTable&);
    const QPair<QString, QString> find_by_username(const QString& username);    
    /// checked select
    bool select();
    /// checked submitAll
    bool submitAll();
    QSqlDatabase database;
    QSqlRelationalTableModel* users;
};

bool hasUser(const QString& username);
bool hasUserWithPassword(const QString& username, const QString& password);
bool addNewUser(const QString& username, const QString& password, 
        const QString& salt);
#endif // USERSTABLE_H

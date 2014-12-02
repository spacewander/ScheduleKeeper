#ifndef NET_H
#define NET_H

#include <QList>
#include <QNetworkAccessManager>
#include <QObject>

#include "remotejournal.h"

class Net : public QObject
{
    Q_OBJECT
public:
    static Net* getNetManager();
    bool isConnectedToNet();
    /**
     * @brief getUser
     * @param username
     * query user with username
     * @return password and salt
     */
    const QPair<QString, QString> getUser(const QString& username);
    /// create User by post his/her data to cloud
    bool postUser(const QString& username, const QString& password,
                  const QString& salt);
    QList<BasicJournal> getBasicJournalList();
    bool addDeletedToBasicJournalList(QList<BasicJournal>& list);
    bool updateBasicJournal(const BasicJournal& journal);
    bool updateDetailJournal(const DetailJournal& journal);
    const BasicJournal findBasicJournal(const QString& journalID);
    const DetailJournal findDetailJournal(const QString& journalID);

signals:

public slots:

private:
    explicit Net(QObject *parent = 0);
    explicit Net(const Net&);
    Net operator =(Net&);

    QNetworkAccessManager* netAccess;
};

bool isConnected();

#endif // NET_H

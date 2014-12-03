#ifndef NET_H
#define NET_H

#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

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
     * @return password and salt
     * query user with username
     */
    QPair<QString, QString> getUser(const QString& username);
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
    /**
     * @brief sendUserInfoBack
     */
    void sendUserInfoBack(const QPair<QString, QString>);
public slots:

private:
    explicit Net(QObject *parent = 0);
    explicit Net(const Net&);
    Net operator =(Net&);
    void setCommonHeader(QNetworkRequest *req);
    bool checkStatusCode(QNetworkReply *res);
    void blockUntilFinished(QNetworkReply* res);
    QJsonArray getJSONResult(QNetworkReply *res);
    QPair<QString, QString>  userFound();

    QNetworkAccessManager* netAccess;
    QUrl userPath;
    QUrl basicJournalPath;
    QUrl detailJournalPath;
    QNetworkReply* resFindUser;
    QNetworkReply* resRegisterUser;
};

bool isConnected();

#endif // NET_H

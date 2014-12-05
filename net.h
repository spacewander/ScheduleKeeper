#ifndef NET_H
#define NET_H

#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QMap>
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

    // all const QString QList should be used to fetch relative journals
    // all non-const Journal QLists should be filled with remote data
    // all const Journal QLists should be sent to remote

    /// 获取要同步的当前所有BasicJournal
    bool getBasicJournalList(QList<BasicJournal>& journals);
    /// 更新要添加delete标记和要修改saveTime的BasicJournal
    bool updateBasicJournal(const QList<BasicJournal>& willPut);
    
    /// 更新要修改的DetailJournal
    bool updateDetailJournal(const QList<DetailJournal> &willPut);
    /**
     * 更新要创建的BasicJournal和要创建的DetailJournal
     * 创建willPostD中的日程后，以获取的objectId更新willPostB中的日程，然后再去创建willPostB
     */
    bool updateRemoteJournal(QList<BasicJournal>& willPostB,
                             const QList<DetailJournal>& willPostD);
    /// 获取要新增的DetailJournal
    bool getDetailJournal(const QList<QString>& objectIds, QList<DetailJournal>& journals);
    /// 获取要合并的DetailJournal
    bool mergeDetailJournal(const QList<QString>& objectIds, 
            QMap<QString, DetailJournal>& journals);

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
    /// used to Get all instances of a class
    QJsonArray getJSONResult(QNetworkReply *res);
    /// used to Get all results of a batch query
    QJsonArray getBatchJSONResult(QNetworkReply *res);
    QPair<QString, QString>  userFound();

    QNetworkAccessManager* netAccess;
    QUrl batchPath;
    QUrl userPath;
    QUrl basicJournalPath;
    QUrl detailJournalPath;
    QNetworkReply* resFindUser;
    QNetworkReply* resRegisterUser;
    QNetworkReply* resGetBasicJournal;
    QNetworkReply* resGetDetailJournal;
    QNetworkReply* resUpdateDetailJournal;
    QNetworkReply* resUpdateBasicJournal;
};

bool isConnected();

#endif // NET_H

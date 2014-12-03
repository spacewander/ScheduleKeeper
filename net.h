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
    /// 更新要添加delete标记和要修改saveTime的以及要创建的BasicJournal
    bool updateBasicJournal(const QList<BasicJournal>& willDelete, 
                            const QList<BasicJournal>& willUpdate,
                            const QList<BasicJournal>& willPost);
    
    /// 更新要创建的DetailJournal和要修改的DetailJournal
    bool updateDetailJournal(const QList<DetailJournal>& willPost,
                             const QList<DetailJournal> &willPut);
    /// 获取要新增的DetailJournal
    bool getDetailJournal(const QList<QString>& objectIds);
    /// 获取要合并的DetailJournal
    bool mergeDetailJournal(const QList<QString>& objectIds);

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

    const QString addDeletedToBasicJournalList(const
                                               QList<BasicJournal>& list);
    const QString updateSaveTimeForBasicJournalList(const
                                               QList<BasicJournal>& list);


    QNetworkAccessManager* netAccess;
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

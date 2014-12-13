#include <QByteArray>
#include <QDebug>
#include <QEventLoop>
#include <QJsonDocument>
#include <QMessageBox>
#include <QUrlQuery>

#include "net.h"

const QString Prefix = "https://leancloud.cn/1.1/";
const QString batchReqPrefix = "/1.1/classes/";
const QByteArray AppIDHeader("X-AVOSCloud-Application-Id" );
const QByteArray AppID("fgcjbm4lgby7lem6erh595k3nduag4l7qs00jbx7xdcixwyt");
const QByteArray AppKeyHeader("X-AVOSCloud-Application-Key");
const QByteArray AppKey("q00yuhfslnqjhvmptppmtxwsu7tsuvefh3wrsowagta9xanx");
const QByteArray MimeHeader("Content-Type");
const QByteArray Mime("application/json");

Net *net = nullptr;

Net* Net::getNetManager()
{
    if (net == nullptr) {
        net = new Net();
    }
    return net;
}

Net::Net(QObject *parent) :
    QObject(parent)
{
    netAccess = new QNetworkAccessManager();
    userPath = QUrl(Prefix + "classes/User");
    basicJournalPath = QUrl(Prefix + "classes/BasicJournal");
    detailJournalPath = QUrl(Prefix + "classes/DetailJournal");
    batchPath = QUrl(Prefix + "batch");
    resFindUser = nullptr;
    resRegisterUser = nullptr;
    resGetBasicJournal = nullptr;
    resGetDetailJournal = nullptr;
    resUpdateBasicJournal = nullptr;
    resUpdateDetailJournal = nullptr;
    resPostBJournal = nullptr;
    resPostDJournal = nullptr;
}

bool Net::isConnectedToNet()
{
    return (netAccess->networkAccessible() == QNetworkAccessManager::Accessible);
}

QPair<QString, QString> Net::getUser(const QString& username)
{
    QUrl findUserPath(userPath);
    QUrlQuery findUserQuery(QString("where={\"username\":\"%1\"}").arg(username));
    findUserPath.setQuery(findUserQuery);
    QNetworkRequest req = QNetworkRequest(findUserPath);
    setCommonHeader(&req);

    resFindUser = netAccess->get(req);
    blockUntilFinished(resFindUser);
    return userFound();
}

bool Net::postUser(const QString& username, const QString& password,
              const QString& salt)
{
    if (username == "")
        return true;

    QNetworkRequest req = QNetworkRequest(QUrl(userPath));
    setCommonHeader(&req);
    QJsonObject user;
    user["username"] = username;
    user["password"] = password;
    user["salt"] = salt;

    resRegisterUser = netAccess->post(req, QJsonDocument(user).toJson());
    blockUntilFinished(resRegisterUser);
    bool status = checkStatusCode(resRegisterUser);
    if (status)
        resRegisterUser->deleteLater();
    return status;
}

bool Net::getBasicJournalList(QList<BasicJournal> &journals,
                              QMap<QString, QString> &journalIdToObjectId)
{
    QNetworkRequest req = QNetworkRequest(basicJournalPath);
    setCommonHeader(&req);

    resGetBasicJournal = netAccess->get(req);
    blockUntilFinished(resGetBasicJournal);
    bool ok = false;
    if ((ok = checkStatusCode(resGetBasicJournal))) {
        QJsonArray json = getJSONResult(resGetBasicJournal);
        for (auto i : json) {
            BasicJournal journal;
            journal.read(i.toObject());
            journals.push_back(journal);
            journalIdToObjectId[journal.journalId] = i.toObject()["objectId"]
                    .toString();
        }
    }
    return ok;
}

bool Net::updateBasicJournal(const QList<BasicJournal>& willPut,
                             const QMap<QString, QString> &journalIdToObjectId)
{
    if (willPut.size() == 0)
        return true;

    QJsonObject batch;
    QJsonArray putBasicJournals;
    for (auto i : willPut) {
        QJsonObject req;
        req["method"] = QString("PUT");
        req["path"] = batchReqPrefix + "BasicJournal/" +
                journalIdToObjectId[i.journalId];
        QJsonObject body;
        i.write(body);
        req["body"] = body;
        putBasicJournals.push_back(req);
    }
    batch["requests"] = putBasicJournals;
    qDebug() << QJsonDocument(batch).toJson();

    QNetworkRequest req = QNetworkRequest(batchPath);
    setCommonHeader(&req);

    resUpdateBasicJournal = netAccess->post(req, QJsonDocument(batch).toJson());
    blockUntilFinished(resUpdateBasicJournal);

    bool ok = false;
    if (checkStatusCode(resUpdateBasicJournal)) {
        ok = ensureRemoteChanged(resUpdateBasicJournal);
    }
    return ok;
}

bool Net::updateDetailJournal(const QList<DetailJournal>& willPut,
                              const QMap<QString, QString> &journalIdToObjectId)
{
    QJsonObject batch;
    QJsonArray putDetailJournals;
    for (auto i : willPut) {
        QJsonObject req;
        req["method"] = QString("PUT");
        req["path"] = batchReqPrefix + "DetailJournal/" +
                journalIdToObjectId[i.journalId];
        QJsonObject body;
        i.write(body);
        req["body"] = body;
        putDetailJournals.push_back(req);
    }
    batch["requests"] = putDetailJournals;

    QNetworkRequest req = QNetworkRequest(batchPath);
    setCommonHeader(&req);

    resUpdateDetailJournal = netAccess->post(req, QJsonDocument(batch).toJson());
    blockUntilFinished(resUpdateDetailJournal);

    bool ok = false;
    if (checkStatusCode(resUpdateDetailJournal)) {
        ok = ensureRemoteChanged(resUpdateDetailJournal);
    }
    return ok;
}

bool Net::updateRemoteJournal(QList<BasicJournal> &willPostB,
                              const QList<DetailJournal> &willPostD)
{
    if (willPostD.size() == 0) {
        return true;
    }

    QMap<QString, QString> journalIdToObjectId;
    bool ok = postDetailJournal(willPostD, journalIdToObjectId);
    if (!ok)
        return ok;
    // remember to set the detailObjectId of BasicJournals
    for (auto i = willPostB.begin(); i != willPostB.end(); ++i) {
        (*i).detailObjectId = journalIdToObjectId[(*i).journalId];
    }
    return postBasicJournal(willPostB);
}

 bool Net::getDetailJournal(const QList<QString>& objectIds,
                            QList<DetailJournal> &journals)
{
    if (objectIds.size() == 0)
        return true;
    QJsonObject batch;
    QJsonArray getDetailJournals;
    for (auto i : objectIds) {
        QJsonObject req;
        req["method"] = QString("GET");
        req["path"] = batchReqPrefix + "DetailJournal/" + i;
        getDetailJournals.push_back(req);
    }
    batch["requests"] = getDetailJournals;

    QNetworkRequest req = QNetworkRequest(batchPath);
    setCommonHeader(&req);

    resGetDetailJournal = netAccess->post(req, QJsonDocument(batch).toJson());
    blockUntilFinished(resGetDetailJournal);

    bool ok = false;
    if ((ok = checkStatusCode(resGetDetailJournal))) {
        QJsonArray json = getBatchJSONResult(resGetDetailJournal);
        for (auto i : json) {
            DetailJournal journal;
            journal.read(i.toObject());
            journals.push_back(journal);
        }
    }
    return ok;
}

bool Net::mergeDetailJournal(const QList<QString>& objectIds, 
        QMap<QString, DetailJournal>& journals)
{
    if (objectIds.size() == 0)
        return true;

    QList<DetailJournal> journalsList;
    bool ok = false;
    ok = getDetailJournal(objectIds, journalsList);
    if (ok) {
        for (auto i : journalsList) {
            journals[i.journalId] = i;
        }
    }
    return ok;
}

bool Net::postDetailJournal(const QList<DetailJournal> &willPostD,
                            QMap<QString, QString> &journalIdToObjectId)
{
    QJsonObject batch;
    QJsonArray postDetailJournals;
    QList<QString> journalList;
    for (auto i : willPostD) {
        journalList.push_back(i.journalId);
        QJsonObject req;
        req["method"] = QString("POST");
        req["path"] = batchReqPrefix + "DetailJournal";
        QJsonObject body;
        i.write(body);
        req["body"] = body;
        postDetailJournals.push_back(req);
    }
    batch["requests"] = postDetailJournals;

    QNetworkRequest req = QNetworkRequest(batchPath);
    setCommonHeader(&req);
    resPostDJournal = netAccess->post(req, QJsonDocument(batch).toJson());
    blockUntilFinished(resPostDJournal);

    bool ok = false;
    if ((ok = checkStatusCode(resPostDJournal))) {
        QJsonArray json = getBatchJSONResult(resPostDJournal);
        int order = 0;
        for (auto i : json) {
            QJsonObject journal(i.toObject());
            QString journalId = journalList.at(order);
            ++order;
            journalIdToObjectId[journalId] = journal["objectId"].toString();
        }
    }
    return ok;
}

bool Net::postBasicJournal(const QList<BasicJournal> &willPostB)
{
    QJsonObject batch;
    QJsonArray postBasicJournals;
    for (auto i : willPostB) {
        QJsonObject req;
        req["method"] = QString("POST");
        req["path"] = batchReqPrefix + "BasicJournal";
        QJsonObject body;
        i.write(body);
        req["body"] = body;
        postBasicJournals.push_back(req);
    }
    batch["requests"] = postBasicJournals;
//    qDebug() << QJsonDocument(batch).toJson();
    QNetworkRequest req = QNetworkRequest(batchPath);
    setCommonHeader(&req);
    resPostBJournal = netAccess->post(req, QJsonDocument(batch).toJson());
    blockUntilFinished(resPostBJournal);

    bool ok = false;
    if (checkStatusCode(resPostBJournal)) {
        ok = ensureRemoteChanged(resPostBJournal);
    }
    return ok;
}

QPair<QString, QString> Net::userFound()
{
    QPair<QString, QString> pass_with_salt;
    if (resFindUser != nullptr && checkStatusCode(resFindUser)) {
        QJsonArray json = getJSONResult(resFindUser);
        pass_with_salt.first = json.first().toObject()["password"].toString();
        pass_with_salt.second = json.first().toObject()["salt"].toString();
    }
    else {
        pass_with_salt.first = "";
        pass_with_salt.second = "";
    }
    return pass_with_salt;
}

void Net::setCommonHeader(QNetworkRequest *req)
{
    req->setRawHeader(AppIDHeader, AppID);
    req->setRawHeader(AppKeyHeader, AppKey);
    req->setRawHeader(MimeHeader, Mime);
}

bool Net::checkStatusCode(QNetworkReply *res)
{
    if (res->error() != QNetworkReply::NoError) {
        qDebug() << res->readAll().data();
        QMessageBox::information(nullptr, tr("Connection"),
                                 tr("Failed: %1").arg(res->errorString()));
        res->deleteLater();
        return false;
    }
    return true;
}

void Net::blockUntilFinished(QNetworkReply *res)
{
    QEventLoop loop;
    connect(res, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

QJsonArray Net::getJSONResult(QNetworkReply *res)
{
    QByteArray content = res->readAll();
    res->deleteLater();
    qWarning() << "Url: " << res->url().toString() << " return: " << content;
    return QJsonDocument::fromJson(content).object()["results"].toArray();
}

QJsonArray Net::getBatchJSONResult(QNetworkReply *res)
{
    QByteArray content = res->readAll();
    res->deleteLater();
    qWarning() << "Batch url: " << res->url().toString() << " return: " << content;
    // if no array received, response will be empty
    QJsonArray response = QJsonDocument::fromJson(content).array();
    QJsonArray results;
    for (auto i : response) {
        // ignore some error results
        if (i.toObject()["success"] != QJsonValue::Undefined) {
            results.push_back(i.toObject()["success"]);
        }
        else {
            qDebug() << "batch error happened: " <<
                        i.toObject()["error"].toObject()["error"].toString();
            results.push_back(QJsonValue::Object);
        }
    }
    return results;
}

bool Net::ensureRemoteChanged(QNetworkReply *res)
{
    QByteArray content = res->readAll();
    res->deleteLater();
    qWarning() << "Batch url: " << res->url().toString() << " return: " << content;
    QJsonArray response = QJsonDocument::fromJson(content).array();
    for (auto i : response) {
        QJsonValue error = i.toObject()["error"];
        if (error.toString() != "") {
            qDebug() << "batch error happened: " <<
                        error.toObject()["error"].toString();
            return false;
        }
    }
    return true;
}

bool isConnected()
{
    Net* net = Net::getNetManager();
    return net->isConnectedToNet();
}

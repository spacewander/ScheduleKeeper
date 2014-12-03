#include <QByteArray>
#include <QDebug>
#include <QEventLoop>
#include <QJsonDocument>
#include <QMessageBox>

#include "net.h"

const QString Prefix = "https://leancloud.cn/1.1/classes/";
const QByteArray AppIDHeader("X-AVOSCloud-Application-Id" );
const QByteArray AppID("a5m764pwiwtgqfskkf9rdaxhn8fre7hszfwxxb9wqa9f3m3a");
const QByteArray AppKeyHeader("X-AVOSCloud-Application-Key");
const QByteArray AppKey("99qovpilikhhin05jpygfcqfqph9fq7apcemik37zn36f1kd");
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
    userPath = QUrl(Prefix + "User");
    basicJournalPath = QUrl(Prefix + "BasicJournal");
    detailJournalPath = QUrl(Prefix + "DetailJournal");
    resFindUser = nullptr;
    resRegisterUser = nullptr;
}

bool Net::isConnectedToNet()
{
    return (netAccess->networkAccessible() == QNetworkAccessManager::Accessible);
}

QPair<QString, QString> Net::getUser(const QString& username)
{
    QNetworkRequest req = QNetworkRequest(QUrl(userPath));
    setCommonHeader(&req);
    // add Query

    resFindUser = netAccess->get(req);
    blockUntilFinished(resFindUser);
    return userFound();
}

bool Net::postUser(const QString& username, const QString& password,
              const QString& salt)
{

}

QList<BasicJournal> Net::getBasicJournalList()
{

}

bool Net::addDeletedToBasicJournalList(QList<BasicJournal>& list)
{

}

bool Net::updateBasicJournal(const BasicJournal& journal)
{

}

bool Net::updateDetailJournal(const DetailJournal& journal)
{

}

const BasicJournal Net::findBasicJournal(const QString& journalID)
{

}

const DetailJournal Net::findDetailJournal(const QString& journalID)
{

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
        QMessageBox::information(nullptr, tr("Connection"),tr("Failed: %1").arg(res->errorString()));
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
    qWarning() << "get user info: " << content;
    return QJsonDocument::fromJson(content).object()["results"].toArray();
}

bool isConnected()
{
    Net* net = Net::getNetManager();
    return net->isConnectedToNet();
}

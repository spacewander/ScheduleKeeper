#include "net.h"

const QString Prefix = " https://leancloud.cn/1.1/classes/";
const QString AppIDHeader = "X-AVOSCloud-Application-Id: {{your_app_id}}";
const QString AppKeyHeader = "X-AVOSCloud-Application-Key: {{your_app_key}}";
const QString Mime = "Content-Type: application/json";

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
}

bool Net::isConnectedToNet()
{
    return (netAccess->networkAccessible() == QNetworkAccessManager::Accessible);
}

const QPair<QString, QString> Net::getUser(const QString& username)
{

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

bool isConnected()
{
    Net* net = Net::getNetManager();
    return net->isConnectedToNet();
}

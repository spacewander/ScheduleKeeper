#include <QJsonObject>
#include <QVariant>
#include <QDebug>

#include "basicjournal.h"
#include "localjournal.h"

BasicJournal::BasicJournal()
{
}

BasicJournal::BasicJournal(const LocalJournal &journal)
{
    journalId = journal.journalId;
    deleted = journal.deleted;
    saveTime = journal.saveTime;
    username = journal.userName;
    detailObjectId = "";
}

void BasicJournal::read(const QJsonObject &json)
{
//    qDebug() << json["journalId"].toDouble();
    journalId = QString("%1").arg(static_cast<qint64>(json["journalId"].toDouble()));
    deleted = json["deleted"].toBool();
    QDateTime mtime;
    mtime.setMSecsSinceEpoch(static_cast<qint64>(json["savetime"].toDouble()));
    saveTime = mtime.toLocalTime();
    username = "a";//json["username"].toString();
    detailObjectId = json["detailobjectId"].toString();
}

void BasicJournal::write(QJsonObject &json) const
{
    json["journalId"] = static_cast<qint64>(journalId.toLongLong());
    json["deleted"] = deleted;
    // convert all time to UTC format before sending to remote
    json["savetime"] = saveTime.toUTC().toMSecsSinceEpoch();
//    json["username"] = username;
    json["detailobjectId"] = detailObjectId;
}

void BasicJournal::deleteSelf()
{
    deleted = true;
    saveTime = QDateTime();
}

// journalId is 10 char fixed length string
bool operator <(const BasicJournal &a, const BasicJournal &b)
{
    return a.journalId < b.journalId;
}

bool operator <(const BasicJournal &a, const LocalJournal &b)
{
    return (a.journalId < b.journalId);
}

bool operator <(const LocalJournal &a, const BasicJournal &b)
{
    return (a.journalId < b.journalId);
}

bool operator ==(const LocalJournal &a, const BasicJournal &b)
{
    return (a.journalId == b.journalId);
}

bool operator !=(const LocalJournal &a, const BasicJournal &b)
{
    return !(a == b);
}

bool operator ==(const BasicJournal &a, const LocalJournal &b)
{
    return (b == a);
}

bool operator !=(const BasicJournal &a, const LocalJournal &b)
{
    return !(b == a);
}

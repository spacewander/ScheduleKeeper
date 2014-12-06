#include <QJsonObject>
#include <QVariant>

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
    journalId = json["journalId"].toString();
    deleted = json["deleted"].toBool();
    saveTime = json["saveTime"].toVariant().toDateTime()
            .toTimeSpec(Qt::LocalTime);
    username = json["username"].toString();
    detailObjectId = json["detailObjectId"].toString();
}

void BasicJournal::write(QJsonObject &json) const
{
    json["journalId"] = journalId;
    json["deleted"] = deleted;
    // convert all time to UTC format before sending to remote
    json["saveTime"] = saveTime.toUTC().toString("yyyy-MM-ddTHH:mm:ss.zzzZ");
    json["username"] = username;
    json["detailObjectId"] = detailObjectId;
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

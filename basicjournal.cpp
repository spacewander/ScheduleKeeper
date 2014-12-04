#include <QJsonObject>

#include "basicjournal.h"

BasicJournal::BasicJournal()
{
}

void BasicJournal::read(const QJsonObject &json)
{
    journalId = json["journalId"].toString();
    deleted = json["deleted"].toBool();
    saveTime = QDateTime::fromString(json["saveTime"].toString());
    username = json["username"].toString();
    detailObjectId = json["detailObjectId"].toString();
}

void BasicJournal::write(QJsonObject &json) const
{
    json["journalId"] = journalId;
    json["deleted"] = deleted;
    json["saveTime"] = saveTime.toString();
    json["username"] = username;
    json["detailObjectId"] = detailObjectId;
}

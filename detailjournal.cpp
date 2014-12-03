#include <QJsonObject>

#include "detailjournal.h"

DetailJournal::DetailJournal()
{
}

void DetailJournal::read(const QJsonObject &json)
{
    journalId = json["journalId"].toString();
    username = json["username"].toString();
    objectId = json["objectId"].toString();
    detail = json["detail"].toString();
    reminder = QDateTime::fromString(json["reminder"].toString());
}

void DetailJournal::write(QJsonObject &json) const
{
    json["journalId"] = journalId;
    json["username"] = username;
    json["objectId"] = objectId;
    json["detail"] = detail;
    json["reminder"] = reminder.toString();
}

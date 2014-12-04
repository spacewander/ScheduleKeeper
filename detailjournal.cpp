#include <QJsonObject>

#include "detailjournal.h"

DetailJournal::DetailJournal()
{
}

void DetailJournal::read(const QJsonObject &json)
{
    journalId = json["journalId"].toString();
    username = json["username"].toString();
    detail = json["detail"].toString();
    if (json["reminder"] != QString("")) {
        reminder = QDateTime::fromString(json["reminder"].toString());
    }
    else {
        reminder = QDateTime();
    }
}

void DetailJournal::write(QJsonObject &json) const
{
    json["journalId"] = journalId;
    json["username"] = username;
    json["detail"] = detail;
    if (reminder.isValid()) {
        json["reminder"] = reminder.toString();
    }
    else {
        json["reminder"] = QString("");
    }
}

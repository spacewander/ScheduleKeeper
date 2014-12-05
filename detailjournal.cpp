#include <QJsonObject>
#include <QVariant>

#include "detailjournal.h"
#include "localjournal.h"

DetailJournal::DetailJournal()
{
}

DetailJournal::DetailJournal(const LocalJournal &journal)
{
    journalId = journal.journalId;
    username = journal.userName;
    detail = journal.detail;
    if (journal.willAlarm) {
        reminder = journal.alarmTime;
    }
    else {
        reminder = QDateTime();
    }
}

void DetailJournal::read(const QJsonObject &json)
{
    journalId = json["journalId"].toString();
    username = json["username"].toString();
    detail = json["detail"].toString();
    if (json["reminder"] != QString("")) {
        reminder = json["reminder"].toVariant().toDateTime();
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

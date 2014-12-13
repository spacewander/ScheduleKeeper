#include <QJsonObject>
#include <QVariant>
#include <QDebug>

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
    journalId = QString("%1").arg(static_cast<qint64>(json["journalId"].toDouble()));
    username = "a";//json["username"].toString();
    detail = json["detail"].toString();
    if (json["reminder"] != QString("")) {
        reminder = json["reminder"].toVariant().toDateTime()
                .toTimeSpec(Qt::LocalTime);
    }
    else {
        reminder = QDateTime();
    }
}

void DetailJournal::write(QJsonObject &json) const
{
    json["journalId"] = static_cast<qint64>(journalId.toLongLong());
//    json["username"] = username;
    json["detail"] = detail;
    if (reminder.isValid()) {
        json["reminder"] = reminder.toUTC().toString("yyyy-MM-dd HH:mm");
    }
    else {
        json["reminder"] = QString("");
    }
}

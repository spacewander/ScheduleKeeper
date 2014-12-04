#ifndef DETAILJOURNAL_H
#define DETAILJOURNAL_H

#include <QDateTime>
#include <QObject>

class DetailJournal
{
public:
    DetailJournal();
    QString journalId;
    QString username;
    QDateTime reminder;
    QString detail;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};

#endif // DETAILJOURNAL_H

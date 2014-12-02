#ifndef BASICJOURNAL_H
#define BASICJOURNAL_H

#include <QDateTime>
#include <QObject>

class BasicJournal
{
public:
    BasicJournal();
    QString journalID;
    bool deleted;
    QDateTime saveTime;
    QString username;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};

#endif // BASICJOURNAL_H

#ifndef BASICJOURNAL_H
#define BASICJOURNAL_H

#include <QDateTime>
#include <QObject>

class BasicJournal
{
public:
    BasicJournal();
    QString journalId;
    bool deleted;
    QDateTime saveTime;
    QString username;
    /**
     * objectId用于更新时作为凭证，不会保存到对应的LocalJournal上
     * 用法：同步时拉下所有的BasicJournal，并记录它们的objectId。
     * 当需要更新云端时（saveTime: local > remote 或者 两者有一个设置了deleted）
     * ，以该objectId作为路径的一部分
     */
    QString detailObjectId;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};

#endif // BASICJOURNAL_H

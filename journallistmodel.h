#ifndef JOURNALLISTMODEL_H
#define JOURNALLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QModelIndex>

#include "localjournal.h"

class JournalListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit JournalListModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    /**
     * @brief removeJournalWithID
     * called when journal is deleted
     */
    bool removeJournalWithID(const QString& journalID);
    /**
     * @brief updateJournal
     * update specific row with new journal data
     */
    bool updateJournal(const LocalJournal& journal);
    bool addJournal(const LocalJournal& journal);
    void setJournals(const QList<LocalJournal>& journals);

    const LocalJournal& at(const unsigned int row) const;
    unsigned int size() const;

signals:

public slots:

private:
    QList<LocalJournal> journals;
};

#endif // JOURNALLISTMODEL_H


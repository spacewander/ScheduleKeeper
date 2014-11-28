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
     * @brief removeRow
     * called when journal is deleted
     */
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    /**
     * @brief updateRow
     * update specific row with new journal data
     */
    bool updateRow(int row, LocalJournal& journal);
signals:

public slots:

private:
    /**
     * @brief fillJournalsList
     * select local journals from sqlite
     */
    void fillJournalsList();
    QList<LocalJournal> journals;
};

#endif // JOURNALLISTMODEL_H


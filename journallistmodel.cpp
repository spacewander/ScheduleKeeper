#include <QDateTime>
#include <QDebug>
#include <QIcon>

#include "journallistmodel.h"

JournalListModel::JournalListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant JournalListModel::data(const QModelIndex &index, int role) const
{
    if ((role != Qt::DisplayRole && role != Qt::DecorationRole)
            || !index.isValid()) {
        return QVariant();
    }
    const int row = index.row();
    const LocalJournal * journal = &journals.at(row);
    if (role == Qt::DecorationRole) {
        if (journal->willAlarm)
            return QIcon(":/rs/alarm-512.png");
        else
            return QIcon(":/rs/null.png");
    }
    else {
        if (journal->willAlarm) {
            return journal->getDetail() + "\n(" + journal->getAlarm() + ")";
        }
        else {
            return journal->getDetail();

        }
    }
}

QVariant JournalListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    (void)section, (void)orientation, (void)role;
    return QVariant();
}

int JournalListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) return 0;
    if (!parent.isValid()) {
        return journals.size();
    }
    return 0;
}

bool JournalListModel::removeRow(int row, const QModelIndex &parent)
{
    if (parent.isValid())
        return false;

    journals.removeAt(row);
    return true;
}

bool JournalListModel::addJournal(const LocalJournal& journal)
{
    journals.push_front(journal);
    return true;
}

bool JournalListModel::updateJournalAtRow(int row, const LocalJournal &journal)
{
    journals[row] = journal;
    return true;
}

void JournalListModel::setJournals(const QList<LocalJournal>& journals)
{
    this->journals = journals;
}


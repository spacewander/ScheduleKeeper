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

bool JournalListModel::removeJournalWithID(const QString& journalId)
{
    auto end = journals.end();
    for (auto it = journals.begin(); it != end; it++) {
        if ((*it).journalId == journalId) {
            journals.erase(it);
            return true;
        }
    }
    return false;
}

bool JournalListModel::addJournal(const LocalJournal& journal)
{
    journals.push_front(journal);
    return true;
}

bool JournalListModel::updateJournal(const LocalJournal &journal)
{
    auto end = journals.end();
    for (auto it = journals.begin(); it != end; it++) {
        if ((*it).journalId == journal.journalId) {
            *it = journal;
            return true;
        }
    }
    return false;
}

void JournalListModel::setJournals(const QList<LocalJournal>& journals)
{
    this->journals = journals;
}

const LocalJournal& JournalListModel::at(const unsigned int row) const
{
    return journals.at(row);
}

unsigned int JournalListModel::size() const
{
    return journals.size();
}

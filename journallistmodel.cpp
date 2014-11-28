#include <QDateTime>
#include <QDebug>
#include <QIcon>
#include <QtSql/QSqlQuery>

#include "journallistmodel.h"

JournalListModel::JournalListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    fillJournalsList();
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

bool JournalListModel::updateRow(int row, LocalJournal &journal)
{
    journals[row] = journal;
    return true;
}

void JournalListModel::fillJournalsList()
{
    // test data begin
    QString testDetail = "生活就像海洋，只有意志坚强的人才能到达彼岸。";
    LocalJournal test1("111111", QDateTime(QDate(2014, 7, 28), QTime(23, 10)),
                       QDateTime(QDate(2014, 11, 27), QTime(22, 18)), testDetail);
    QString test2Detail = "Ent_evo #imaginature# 一个世纪过去了，蚯蚓的王国又向大地的尽头前进了一公里。它们不急。从来都不急。";
    LocalJournal test2("111111", QDateTime(QDate(2014, 10, 28), QTime(3, 10)),
                       QDateTime(QDate(2014, 11, 27), QTime(12, 48)), test2Detail,
                       QDateTime(QDate(2014, 12, 1), QTime(12, 0)));
    journals.push_back(test1);
    journals.push_back(test2);
    // test data end
}

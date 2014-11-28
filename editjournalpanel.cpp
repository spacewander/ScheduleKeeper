#include <QGridLayout>
#include <QIcon>

#include "editjournalpanel.h"
#include "localjournal.h"

EditJournalPanel::EditJournalPanel(QWidget *parent) :
    QWidget(parent)
{
    // UI
    detailEdit = new QPlainTextEdit(this);

    saveBtn = new QPushButton(this);
    saveBtn->setText(tr("保存"));
    connect(saveBtn, &QPushButton::clicked, [&](){
        if (editingJournal.journalID == "") {
            createJournal();
        }
        else {
            saveJournal();
        }
    });
    deleteBtn = new QPushButton(this);
    deleteBtn->setText(tr("删除"));
    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(deleteJournal()));
    alarmBtn = new QPushButton(this);
    alarmBtn->setIcon(QIcon(":/alarm-512.png"));
    connect(alarmBtn, SIGNAL(clicked()), this, SLOT(showAlarmEdit()));
    
    QGridLayout *btnLayout = new QGridLayout();
    btnLayout->addWidget(alarmBtn, 0, 0);
    btnLayout->addWidget(saveBtn, 0, 3);
    btnLayout->addWidget(deleteBtn, 0, 4);

    alarmEdit = new QDateTimeEdit(this);
    alarmEdit->setVisible(false);

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(detailEdit);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(alarmEdit);
    setLayout(mainLayout);
}

EditJournalPanel::~EditJournalPanel()
{
}

void EditJournalPanel::editLocalJournal(const LocalJournal& journal)
{
    editingJournal = journal;
    if (journal.willAlarm) {
        alarmEdit->setVisible(true);
        alarmEdit->setDateTime(journal.alarmTime);
    }
    detailEdit->setPlainText(journal.detail);
}

void EditJournalPanel::showAlarmEdit()
{
    alarmEdit->setDateTime(QDateTime::currentDateTime());
    alarmEdit->setVisible(true);
}

void EditJournalPanel::deleteJournal()
{
    // if there is journal editing
    if (!editingJournal.isDeleted) {
        editingJournal.clear();
        initEditState();
        emit deleteLocalJournal(editingJournal.journalID);
    }
}

void EditJournalPanel::initEditState()
{
    detailEdit->clear();
    alarmEdit->setVisible(false);
}

void EditJournalPanel::saveJournal()
{
    editingJournal.detail = detailEdit->toPlainText();
    if (alarmEdit->isVisible()) {
        editingJournal.willAlarm = true;
        editingJournal.alarmTime = alarmEdit->dateTime();
    }
    editingJournal.saveTime = QDateTime::currentDateTime();
    emit saveLocalJournal(editingJournal);
}

void EditJournalPanel::createJournal()
{
    editingJournal.isDeleted = false;
    editingJournal.createdTime = QDateTime::currentDateTime();
    editingJournal.saveTime = editingJournal.createdTime;
    editingJournal.detail = detailEdit->toPlainText();
    if (alarmEdit->isVisible()) {
        editingJournal.willAlarm = true;
        editingJournal.alarmTime = alarmEdit->dateTime();
    }
    emit createLocalJournal(editingJournal);
}


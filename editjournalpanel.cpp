#include <QGridLayout>
#include <QIcon>

#include "editjournalpanel.h"
#include "localjournal.h"

EditJournalPanel::EditJournalPanel(QWidget *parent) :
    QWidget(parent)
{
    // UI
    detailLabel = new QLabel(tr("记下你的日程吧"));
    detailEdit = new QPlainTextEdit(this);
    detailEdit->setFocus();

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
    alarmBtn->setIcon(QIcon(":/rs/alarm-512.png"));
    connect(alarmBtn, SIGNAL(clicked()), this, SLOT(showAlarmEdit()));
    
    QGridLayout *btnLayout = new QGridLayout();
    btnLayout->addWidget(alarmBtn, 0, 0, 1, 1);
    QLabel *null = new QLabel("");
    btnLayout->addWidget(null, 0, 1, 1, 5);
    btnLayout->addWidget(saveBtn, 0, 6, 1, 2);
    btnLayout->addWidget(deleteBtn, 0, 8, 1, 2);

    alarmEdit = new QDateTimeEdit(this);
    alarmEdit->setDisplayFormat("yyyy年MM月dd日 hh时mm分");
    alarmEdit->setVisible(false);

    mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(20, 0, 0, 0);
    mainLayout->addWidget(detailLabel);
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
    else {
        alarmEdit->setVisible(false);
    }
    detailEdit->setPlainText(journal.detail);
}

void EditJournalPanel::startNewJournal()
{
    initEditState();
    this->setFocus();
}

void EditJournalPanel::showAlarmEdit()
{
    if (alarmEdit->isVisible()) {
        alarmEdit->setVisible(false);
    }
    else {
        alarmEdit->setDateTime(QDateTime::currentDateTime());
        alarmEdit->setVisible(true);
    }
}

void EditJournalPanel::deleteJournal()
{
    // if there is journal editing
    if (!editingJournal.deleted) {
        emit deleteLocalJournal(editingJournal.journalID);
        editingJournal.clear();
        initEditState();
    }
}

void EditJournalPanel::initEditState()
{
    detailEdit->clear();
    alarmEdit->setVisible(false);
    editingJournal = LocalJournal();
}

void EditJournalPanel::saveJournal()
{
    if (editingJournal.deleted) {
        return;
    }
    editingJournal.detail = detailEdit->toPlainText();
    if (alarmEdit->isVisible()) {
        editingJournal.willAlarm = true;
        editingJournal.alarmTime = alarmEdit->dateTime();
    }
    else {
        editingJournal.willAlarm = false;
    }
    editingJournal.saveTime = QDateTime::currentDateTime();
    emit saveLocalJournal(editingJournal);
}

void EditJournalPanel::createJournal()
{
    editingJournal.generalizeID();
    editingJournal.deleted = false;
    editingJournal.createdTime = QDateTime::currentDateTime();
    editingJournal.saveTime = editingJournal.createdTime;
    editingJournal.detail = detailEdit->toPlainText();
    if (alarmEdit->isVisible()) {
        editingJournal.willAlarm = true;
        editingJournal.alarmTime = alarmEdit->dateTime();
    }
    emit createLocalJournal(editingJournal);
}


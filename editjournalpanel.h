#ifndef EDITJOURNALPANEL_H
#define EDITJOURNALPANEL_H

#include <QDateTime>
#include <QDateTimeEdit>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "localjournal.h"

/**
 * @brief The EditJournalPanel class
 * The panel used to create/edit/delete journal
 */
class EditJournalPanel : public QWidget
{
    Q_OBJECT

public:
    explicit EditJournalPanel(QWidget *parent = 0);
    ~EditJournalPanel();
    void editLocalJournal(const LocalJournal& journal);
    /**
     * @brief initEditState
     * 初始化面板到创建/编辑日程之前的状态
     */
    void initEditState();

public slots:
    void startNewJournal();

signals:
    void deleteLocalJournal(const QString& journalId);
    void saveLocalJournal(const LocalJournal& journal);
    void createLocalJournal(LocalJournal& journal);

private slots:
    void showAlarmEdit();

    void deleteJournal();

private:
    void saveJournal();
    void createJournal();

    LocalJournal editingJournal;

    QDateTimeEdit *alarmEdit;

    QPushButton *saveBtn;
    QPushButton *deleteBtn;
    QPushButton *alarmBtn;

    QLabel *detailLabel;
    QPlainTextEdit *detailEdit;
    QVBoxLayout *mainLayout;
};

#endif // EDITJOURNALPANEL_H

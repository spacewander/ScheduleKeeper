#ifndef EDITJOURNALPANEL_H
#define EDITJOURNALPANEL_H

#include <QDateTime>
#include <QDateTimeEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class LocalJournal;

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

signals:
    void deleteLocalJournal(const QString& journalID);
    void saveLocalJournal(const LocalJournal& journal);
    void createLocalJournal(const LocalJournal& journal);

private:
    void showAlarmEdit();

    void deleteJournal();
    /**
     * @brief initEditState
     */
    void initEditState();
    void saveJournal();
    void createJournal();

    LocalJournal editingJournal;

    QDateTimeEdit *alarmEdit;

    QPushButton *saveBtn;
    QPushButton *deleteBtn;
    QPushButton *alarmBtn;

    QPlainTextEdit *detailEdit;
    QVBoxLayout *mainLayout;
};

#endif // EDITJOURNALPANEL_H

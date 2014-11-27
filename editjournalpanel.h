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
    void editLocalJournal(LocalJournal& journal);

signals:
    void deleteLocalJournal(const QString& journalID);
    LocalJournal& saveLocalJournal();
    LocalJournal& createLocalJournal();

private:
    void showAlarmEdit();
    /**
     * @brief setAlarm
     * @param datetime
     *
     * set alarm and turn isRemainded to true
     */
    void setAlarm(const QDateTime& datetime);
    void deleteJournal();
    void saveJournal();
    void createJournal();

    QString journalID;

    /**
     * @brief alarm
     * store current alarm time, if not given, use current time instead
     */
    QDateTime *alarm;
    QDateTimeEdit *alarmEdit;
    bool isRemainded;

    QPushButton *saveBtn;
    QPushButton *deleteBtn;
    QPushButton *alarmBtn;

    QPlainTextEdit *detailEdit;
};

#endif // EDITJOURNALPANEL_H

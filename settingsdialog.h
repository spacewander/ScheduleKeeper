#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);

signals:

private slots:
    void changeAutoUpdateSetting();

private:
    QSettings settings;
    QDialogButtonBox *buttons;
    QRadioButton *autoUpdateRadioBtn;
    QRadioButton *manualUpdateRadioBtn;
    QGroupBox *radioBtnGroupBox;
};

#endif // SETTINGSDIALOG_H


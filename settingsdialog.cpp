#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    settings("ScheduleKeeper", "ScheduleKeeper")
{
    setWindowTitle(tr("设置"));
    radioBtnGroupBox = new QGroupBox(tr("更新方式"));
    autoUpdateRadioBtn = new QRadioButton(tr("自动更新"));
    manualUpdateRadioBtn = new QRadioButton(tr("手动更新"));
    const bool selected = settings.value("autoupdate", true).toBool();
    if (selected) {
        autoUpdateRadioBtn->setChecked(true);
    }
    else {
        manualUpdateRadioBtn->setChecked(false);
    }

    QHBoxLayout *groupBoxLayout = new QHBoxLayout(this);
    groupBoxLayout->addWidget(autoUpdateRadioBtn);
    groupBoxLayout->addWidget(manualUpdateRadioBtn);
    radioBtnGroupBox->setLayout(groupBoxLayout);
    
    buttons = new QDialogButtonBox(this);
    buttons->addButton( QDialogButtonBox::Ok );
    buttons->addButton(QDialogButtonBox::Cancel );
    buttons->button(QDialogButtonBox::Ok)->setText(tr("保存"));
    buttons->button(QDialogButtonBox::Cancel)->setText(tr("取消"));
    connect( buttons->button( QDialogButtonBox::Cancel ), SIGNAL(clicked()), this,
             SLOT(close()));
    connect( buttons->button( QDialogButtonBox::Ok ), SIGNAL(clicked()), this,
             SLOT(changeAutoUpdateSetting()) );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(radioBtnGroupBox);
    mainLayout->addWidget(buttons);
    setLayout(mainLayout);
}

void SettingsDialog::changeAutoUpdateSetting()
{
    qDebug() << "before setting: autoupdate" << settings.value("autoupdate").toBool();
    if (autoUpdateRadioBtn->isChecked()) {
        settings.setValue("autoupdate", true);
    }
    else {
        settings.setValue("autoupdate", false);
    }
    qDebug() << "after setting: autoupdate" << settings.value("autoupdate").toBool();
    accept();
}


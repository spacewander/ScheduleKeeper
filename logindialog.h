#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QDebug>

#include "logindialog.h"

class LoginDialog: public QDialog
{
    Q_OBJECT

private:
        QLabel* labelUsername;
        QLabel* labelPassword;
        QLabel* noticeLabel;
        QLineEdit* editUsername;
        QLineEdit* editPassword;
        QDialogButtonBox* buttons;

        void setUpGUI();
        bool canLogin(const QString& username, const QString& password);

public:
    explicit LoginDialog(QWidget *parent = 0);
    void setUsername( const QString& username );
    void setPassword( const QString& password );
    void setUsernamesList( const QStringList& usernames );

public slots:
    void slotAcceptLogin();
};

#endif // LOGINDIALOG_H

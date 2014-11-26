#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QDebug>

#include "logindialog.h"

class MainWindow;

class LoginDialog: public QDialog
{
    Q_OBJECT

private:
        MainWindow* host;

        QLabel* labelUsername;
        QLabel* labelPassword;
        QLabel* labelEnsurePassword;
        QLabel* noticeLabel;
        QLineEdit* editUsername;
        QLineEdit* editPassword;
        QLineEdit* ensurePassword;
        QDialogButtonBox* buttons;

        void setUpGUI();
        bool canLogin(const QString& username, const QString& password);
        bool canRegister(const QString& username, const QString& password);
        void storeUsernameToSetting(const QString& username);

public:
    explicit LoginDialog(QWidget *parent = 0);
    void bindMainwindow(MainWindow* mainwindow);
    void setUsername( const QString& username );
    void setPassword( const QString& password );
    void setUsernamesList( const QStringList& usernames );

public slots:
    void slotAcceptLogin();
    void registerUser();
};

#endif // LOGINDIALOG_H

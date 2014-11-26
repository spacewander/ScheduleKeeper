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
        /**
         * @brief canLogin
         * 是否能够登录。第一步，检查本地的User表，看看用户名和密码是否匹配
         * 第二步，如果用户名不存在，那么访问远程服务，看看用户名和密码是否匹配
         * 如果没有网络，或者密码和用户名不匹配，或者用户不存在，返回失败
         * @param username
         * @param password
         * @return
         */
        bool canLogin(const QString& username, const QString& password);
        /**
         * @brief canRegister
         * 是否能够注册。必须有网络才能使用。然后访问远程服务，提交用户信息。
         * @param username
         * @param password
         * @return
         */
        bool canRegister(const QString& username, const QString& password);
        void storeUsernameToSetting(const QString& username);

public:
    explicit LoginDialog(QWidget *parent = 0);
   /**
     * @brief bindMainwindow
     * 绑定主窗口，这样就能调用setUsername设置主窗口的用户名
     * @param mainwindow
     */
    void bindMainwindow(MainWindow* mainwindow);
    void setUsername( const QString& username );
    void setPassword( const QString& password );
    void setUsernamesList( const QStringList& usernames );

public slots:
    void slotAcceptLogin();
    void registerUser();
};

#endif // LOGINDIALOG_H

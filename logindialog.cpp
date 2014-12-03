#include <QByteArray>
#include <QSettings>

#include "global.h"
#include "logindialog.h"
#include "mainwindow.h"
#include "userstable.h"
#include "net.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent)
{
    setUpGUI();
    setWindowTitle(tr("欢迎使用") );
    setModal(true);
}

// 绑定Mainwindow，这样我们就能直接把用户名传递给它了……懒得重复解码用户名
void LoginDialog::bindMainwindow(MainWindow* mainwindow)
{
    host = mainwindow;
}

void LoginDialog::setUpGUI()
{
    // set up the layout
    QGridLayout* formGridLayout = new QGridLayout( this );

    // initialize the username lineedit
    editUsername = new QLineEdit( this );
    editUsername->setMaxLength(20);
    // initialize the password field so that it does not echo
    // characters
    editPassword = new QLineEdit( this );
    editPassword->setEchoMode( QLineEdit::Password );
    editPassword->setMaxLength(20);
    ensurePassword = new QLineEdit( this );
    ensurePassword->setEchoMode( QLineEdit::Password );
    ensurePassword->setMaxLength(20);
    ensurePassword->setVisible(false);

    // initialize the labels
    labelUsername = new QLabel( this );
    labelPassword = new QLabel( this );
    labelEnsurePassword = new QLabel( this );
    labelUsername->setText( tr( "用户名" ) );
    labelUsername->setBuddy( editUsername );
    labelPassword->setText( tr( "密码" ) );
    labelPassword->setBuddy( editPassword );
    labelEnsurePassword->setText( tr( "确认密码" ) );
    labelEnsurePassword->setBuddy( ensurePassword );
    labelEnsurePassword->setVisible(false);



    // initialize buttons
    buttons = new QDialogButtonBox( this );
    buttons->addButton( QDialogButtonBox::Ok );
    buttons->addButton( QDialogButtonBox::Yes );
    buttons->addButton( QDialogButtonBox::Cancel );
    buttons->button( QDialogButtonBox::Ok )->setText( tr("登录") );
    buttons->button( QDialogButtonBox::Yes )->setText( tr("注册") );
    buttons->button( QDialogButtonBox::Cancel )->setText( tr("退出") );

    // connects slots
    connect( buttons->button( QDialogButtonBox::Cancel ), SIGNAL(clicked()), this,
             SLOT(close()));

    connect( buttons->button( QDialogButtonBox::Ok ), SIGNAL(clicked()), this,
             SLOT(slotAcceptLogin()) );
    connect( buttons->button( QDialogButtonBox::Yes ), SIGNAL(clicked()), this,
             SLOT(registerUser()) );

    noticeLabel = new QLabel(tr("密码不对或用户不存在"));
    QFont font;
    font.setBold(true);
    noticeLabel->setFont(font);
    noticeLabel->setStyleSheet("QLabel { background-color : yellow; color : red; }");
    noticeLabel->setVisible(false);

    // place components into the dialog

    formGridLayout->addWidget(noticeLabel, 0, 0, 1, 2);
    formGridLayout->addWidget( labelUsername, 1, 0 );
    formGridLayout->addWidget( editUsername, 1, 1 );
    formGridLayout->addWidget( labelPassword, 2, 0 );
    formGridLayout->addWidget( editPassword, 2, 1 );
    formGridLayout->addWidget( labelEnsurePassword, 3, 0 );
    formGridLayout->addWidget( ensurePassword, 3, 1 );
    formGridLayout->addWidget( buttons, 4, 1, 1, 3 );

    setLayout( formGridLayout );


}

void LoginDialog::slotAcceptLogin()
{
    QString username = editUsername->text();
    QString password = editPassword->text();

    bool isLogined = canLogin(username, password);
    if (isLogined) {
        storeUsernameToSetting(username);
        // close this dialog with QDialog::Accepted
        accept();
    }
    else {
        editPassword->clear();
        noticeLabel->setVisible(true);
    }
}

bool LoginDialog::canLogin(const QString& username, const QString& password)
{
    // 判断是否存在于本地
    if (hasUserWithPassword(username, password)) {
        return true;
    }
    // 否则在有网络的情况下，发送用户名和加密后的密码到云端进行验证
    if (isConnected()) {
//        noticeLabel->setText(tr("正在尝试登录……"));
//        noticeLabel->setVisible(true);
        QPair<QString, QString> pass_with_salt = Net::getNetManager()->getUser(username);
        if (pass_with_salt.first == "") {
            return false;
        }
        // 存储用户信息到本地
        storeUserToDB(username, pass_with_salt.first, pass_with_salt.second);
        if (hasUserWithPassword(username, password)) {
            return true;
        }
    }
    return false;
}

void LoginDialog::storeUsernameToSetting(const QString& username)
{
    QSettings setting("ScheduleKeeper", "ScheduleKeeper");
    host->setUsername(username);
    QByteArray sessionUser((SESSSION_KEY + username).toStdString().c_str());

    setting.setValue("username", sessionUser.toBase64(QByteArray::Base64Encoding));
    qDebug() << QByteArray::fromBase64(setting.value("username").toByteArray())
        .data();
}

void LoginDialog::storeUserToDB(const QString& username, const QString& password, 
        const QString& salt)
{
    addNewUser(username, password, salt);
}

void LoginDialog::registerUser()
{
    if (buttons->button(QDialogButtonBox::Yes)->text() == tr("注册")) {
        buttons->button(QDialogButtonBox::Yes)->setText(tr("确认"));
        buttons->button(QDialogButtonBox::Ok)->setVisible(false);
        ensurePassword->setVisible(true);
        labelEnsurePassword->setVisible(true);
        return;
    }

    QString username = editUsername->text();
    QString password = editPassword->text();
    QString passwordRepeated = ensurePassword->text();
    if (passwordRepeated != password) {
        noticeLabel->setText(tr("密码不对应啊"));
        noticeLabel->setVisible(true);
        return;
    }

    bool isRegistered = canRegister(username, password);
    if (isRegistered) {
        storeUsernameToSetting(username);
        // close this dialog with QDialog::Accepted
        accept();
    }
    else {
        editPassword->clear();
        noticeLabel->setText(tr("注册失败，需要有网络"));
        noticeLabel->setVisible(true);
    }
}

bool LoginDialog::canRegister(const QString& username, const QString& password)
{
    if (username == "a" && password == "b") {
        return true;
    }
    return false;
}

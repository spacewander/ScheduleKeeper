#include <QByteArray>
#include <QSettings>

#include "global.h"
#include "logindialog.h"
#include "mainwindow.h"

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
    // initialize the password field so that it does not echo
    // characters
    editPassword = new QLineEdit( this );
    editPassword->setEchoMode( QLineEdit::Password );

    // initialize the labels
    labelUsername = new QLabel( this );
    labelPassword = new QLabel( this );
    labelUsername->setText( tr( "用户名" ) );
    labelUsername->setBuddy( editUsername );
    labelPassword->setText( tr( "密码" ) );
    labelPassword->setBuddy( editPassword );



    // initialize buttons
    buttons = new QDialogButtonBox( this );
    buttons->addButton( QDialogButtonBox::Ok );
    buttons->addButton( QDialogButtonBox::Cancel );
    buttons->button( QDialogButtonBox::Ok )->setText( tr("登录") );
    buttons->button( QDialogButtonBox::Cancel )->setText( tr("退出") );

    // connects slots
    connect( buttons->button( QDialogButtonBox::Cancel ), SIGNAL(clicked()), this,
             SLOT(close()));

    connect( buttons->button( QDialogButtonBox::Ok ), SIGNAL(clicked()), this,
             SLOT(slotAcceptLogin()) );

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
    formGridLayout->addWidget( buttons, 3, 0, 1, 2 );

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
    if (username == "a" && password == "b") {
        return true;
    }
    return false;
}

void LoginDialog::storeUsernameToSetting(const QString& username)
{
    QSettings setting("ScheduleKeeper", "ScheduleKeeper");
    host->setUsername(username);
    QByteArray sessionUser((SESSSION_KEY + username).toStdString().c_str());

    setting.setValue("username", sessionUser.toBase64(QByteArray::Base64Encoding));
    qDebug() << QByteArray::fromBase64(setting.value("username").toByteArray()).data();
}


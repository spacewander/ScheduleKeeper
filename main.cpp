#include <QApplication>
#include <QByteArray>
#include <QDebug>
#include <QSettings>
#include <QString>

#include "global.h"
#include "mainwindow.h"
#include "logindialog.h"

/**
 * @brief extractUsername
 * @param ok    如果能够正确读取用户名，设置为true，否则为false。在处理返回结果之前必须检查这一项是否为true
 * @return  解码后的用户名
 */
const QString extractUsername(bool &ok)
{
    QSettings setting("ScheduleKeeper", "ScheduleKeeper");
    QString encodedUsername = 
        QByteArray::fromBase64(setting.value("username").toByteArray()).data();
    qDebug() << "current username: " << encodedUsername;
    if (encodedUsername == "" || !encodedUsername.startsWith(SESSSION_KEY)) {
        ok = false;
        return "";
    }
    ok = true;
    return encodedUsername.replace(0, SESSSION_KEY.size(), "");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("ScheduleKeeper");
    MainWindow w;
    bool hasCurrentUser = false;
    const QString currentUser = extractUsername(hasCurrentUser);
    if (hasCurrentUser) {
        w.setUsername(currentUser);
        w.setUpGUI();
        w.showMaximized();
    }
    else {
        LoginDialog loginDialog;
        loginDialog.bindMainwindow(&w);
        if (loginDialog.exec() == QDialog::Accepted) {
            //w.show();
            w.setUpGUI();
            w.showMaximized();
        }
        else {
            w.close();
        }
    }

    return a.exec();
}

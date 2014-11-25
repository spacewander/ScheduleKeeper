#include <QApplication>

#include "mainwindow.h"
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("ScheduleKeeper");
    MainWindow w;
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

    return a.exec();
}

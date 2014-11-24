#include <QApplication>

#include "mainwindow.h"
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    LoginDialog loginDialog;
    if (loginDialog.exec() == QDialog::Accepted) {
        w.show();
    }
    else {
        w.close();
    }

    return a.exec();
}

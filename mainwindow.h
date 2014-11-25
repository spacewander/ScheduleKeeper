#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QListView>
#include <QMainWindow>
#include <QSettings>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setUpGUI();
    void setUsername(const QString& username);

private:
    QString username;
    // set layout to QMainWindow
    QWidget *centralWidget;
    QSettings settings;

    QVBoxLayout *toolbarLayout;
    QLabel *usernameLabel;
    QPushButton *settingButton;

    QListView *journalListView;

    QGridLayout *mainLayout;
};

#endif // MAINWINDOW_H

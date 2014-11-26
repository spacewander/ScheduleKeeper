#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QToolBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setUpGUI();
    void setUsername(const QString& username);

private:
    void refreshLastUpdateTime();

    QString username;
    // set layout to QMainWindow
    QWidget *centralWidget;
    QSettings settings;

    QToolBar *toolbar;
    QLabel *usernameLabel;
    QAction *settingAction;

    QMenu *sortMenu;
    QAction *sortBySaveTime;
    QAction *sortByCreatedTime;
    QAction *sortByRemainder;

    QAction *newAction;
    QLineEdit *searchEdit;
    QAction *clearSearchResultAction;

    QAction *updateAction;
    QLabel *lastUpdateLabel;

    QListView *journalListView;

    QGridLayout *mainLayout;
};

#endif // MAINWINDOW_H

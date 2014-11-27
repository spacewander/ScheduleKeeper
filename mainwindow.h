#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDateTime>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QToolBar>

#include "settingsdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setUpGUI();
    /**
     * @brief setUsername
     * @param username 给主窗口一个用户名，在主窗口显示之前，先获取用户名（从本地设置或者用户输入），再把用户名传递给它
     */
    void setUsername(const QString& username);

private slots:
    void enableClearSearch(const QString &text);

private:
    /**
     * @brief refreshLastUpdateTime
     * 更新最近一次更新的时间，会修改相关的UI
     */
    void refreshLastUpdateTime();
    QDateTime lastUpdateTime;

    QString username;
    /**
     * set layout to QMainWindow
     */
    QWidget *centralWidget;
    /**
     *  配置名: 组织名 ScheduleKeeper 程序名 ScheduleKeeper
     *  当前保存的项目：
     *  1. username
     *  2. autoupdate
     *  3. lastupdatetime
     */
    QSettings settings;
    SettingsDialog settingsDialog;

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

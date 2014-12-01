#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDateTime>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QListView>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QToolBar>

#include "settingsdialog.h"
#include "localjournal.h"
#include "journallistmodel.h"

class LocalJournal;
class EditJournalPanel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setup();

    /**
     * @brief setUsername
     * @param username 给主窗口一个用户名，在主窗口显示之前，先获取用户名（从本地设置或者用户输入），再把用户名传递给它
     */
    void setUsername(const QString& username);

public slots:
    void deleteLocalJournal(const QString& journalID);
    void saveLocalJournal(const LocalJournal&);
    void createLocalJournal(const LocalJournal&);
    void logout();

private slots:
    void enableClearSearch(const QString &text);
    /**
     * 获取ListView中的index对应的LocalJournal数据，并存储到EditJournalPanel中
     * 额外存row到editingRow成员变量中，作为对应的操作的连接
     */
    void getJournalWithIndex(const QModelIndex &index);
    /**
     *  清除搜索框的内容，并将EditJournalPanel设置为创建日程的状态
     */
    void startNewJournal();
    /**
     * 清除搜索信息，展示所有的LocalJournal
     */
    void switchSearchToDisplay();
    void updateJournals();

private:
    void setUpGUI();
    void setUpJournals();
    void connectEditJournalPanel();

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
    QAction *logoutAction;
    QLabel *usernameLabel;

    QAction *settingAction;

    QMenu *sortMenu;
    QAction *sortBySaveTime;
    QAction *sortByCreatedTime;
    QAction *sortByRemainder;

    QAction *newAction;
    QLineEdit *searchEdit;
    bool isSearched;
    QAction *clearSearchResultAction;

    QAction *updateAction;
    QLabel *lastUpdateLabel;

    /**
     *  展示全部的LocalJournal（排列顺序会不同），或展示符合筛选要求的LocalJournal
     */
    QListView *journalListView;
    // listModel for journalListView
    JournalListModel journalList;

    /**
     *  作为各种对ListModel的操作的连接（因为这些操作都需要有一个row参数）
     */
    int editingRow;
    QList<LocalJournal> totalLocalJournals;

    /**
     * 编辑LocalJournal的面板
     */
    EditJournalPanel *editJournalPanel;

    QGridLayout *mainLayout;
};

#endif // MAINWINDOW_H

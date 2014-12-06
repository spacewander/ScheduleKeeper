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
#include <QMap>
#include <QMenu>
#include <QSettings>
#include <QToolBar>
#include <QToolButton>

#include "settingsdialog.h"
#include "global.h"
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
    void deleteLocalJournal(const QString& journalId);
    void saveLocalJournal(const LocalJournal&);
    void createLocalJournal(LocalJournal &);
    void logout();

private slots:
    void startSearching();
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
    /**
     *  更新逻辑：
     *  0. 拉取所有的BasicJournal # getBasicJournalList
     *  1. 排序LocalJournal和BasicJournal，然后遍历两个列表
     *  2. 对于在Local而不在Basic的，如果没有deleted标记，加入到willPost列表
     *  3. 对于在Basic而不在Local的，如果没有deleted标记，
     *  将对应的DetailJournal的objectId加入到willGet列表
     *  4. 对于两个都在的journal，如下处理
     *  4.1 如果任意一个有deleted标记，对另一个也做delete标记处理。
     *  如果是Basic，加入到willDelete列表
     *  4.2 如果Local的saveTime比Basic的saveTime更晚，将Basic加入到willUpdate列表，将对应的DetailJournal的objectId加入willPut列表
     *  4.3 如果Local的saveTime比Basic的saveTime更早，更新Local的saveTime，
     *  将对应的DetailJournal的objectId加入到willMerge列表
     *  5 分别调用函数来处理willPost\willGet\willDelete\willUpdate\willPut\willMerge
     *  willGet : getDetailJournal
     *  willPut willPost : updateDetailJournal
     *  willMerge : mergeDetailJournal
     *  willDelete willUpdate : updateBasicJournal
     *  6 四个函数都处理完毕后，发布同步完毕的信息
     */
    void updateJournals();

private:
    void setUpGUI();
    void setUpJournals();
    void updateFailed(const QString& msg = tr("无法获取云端日程")) const;
    void connectEditJournalPanel();
    void sortJournalsBy(SortBy sortBy);

    // 同步的辅助函数
    /**
     * @brief refreshLastUpdateTime
     * 更新最近一次更新的时间，会修改相关的UI
     */
    void refreshLastUpdateTime();
    bool deleteLocalInDB(const QList<LocalJournal>& shouldDelete);
    bool updateLocalInDB(const QMap<QString, LocalJournal>& shouldMerge);
    bool insertLocalInDB(const QMap<QString, LocalJournal>& shouldGet);
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

    QToolButton* popupBtn;
    QMenu *sortMenu;
    QAction *sortBySaveTime;
    QAction *sortByRemainder;
    SortBy sortBy;

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
    QList<LocalJournal> relativeLocalJournals;

    /**
     * 编辑LocalJournal的面板
     */
    EditJournalPanel *editJournalPanel;

    QGridLayout *mainLayout;
};

#endif // MAINWINDOW_H

#include <algorithm>
#include <QDebug>
#include <QIcon>
#include <QMessageBox>
#include <QtSql>

#include "mainwindow.h"
#include "editjournalpanel.h"
#include "logindialog.h"
#include "journalstable.h"
#include "net.h"
#include "remotejournal.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    settings("ScheduleKeeper", "ScheduleKeeper"),
    settingsDialog(parent)
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // set up all the core data
    lastUpdateTime = QDateTime::currentDateTime();

    QIcon icon = QIcon(":/rs/alarm-512.png");
    setWindowIcon(icon);
    isSearched = false;
    sortBy = SortByNull;
}

MainWindow::~MainWindow()
{

}

void MainWindow::setUsername(const QString& username)
{
    this->username = username;
}

void MainWindow::setup()
{
    setUpGUI();
    setUpJournals();
    showMaximized();
    const bool selected = settings.value("autoupdate", true).toBool();
    if (selected) {
        updateJournals();
    }
}

void MainWindow::setUpGUI()
{
    // 顶部为工具栏，宽度跟最长的用户名（20个汉字）相对。
    // 从左到右分别为用户名、设置、搜索、排序、添加、更新、更新情况
    // 第一列为日程列表
    // 第二列为日程编辑界面
    mainLayout = new QGridLayout(centralWidget);

    journalListView = new QListView(this);
    editJournalPanel = new EditJournalPanel(this);
    editJournalPanel->setFocus();
    connectEditJournalPanel();

    toolbar = addToolBar("");
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setContentsMargins(100, 0, 0, 0);
    toolbar->setCursor(Qt::PointingHandCursor);
    toolbar->setStyleSheet("QToolBar {\
            background: rgba(255,255,255,1);\
            }");

    qWarning() << "username: " << username;
    usernameLabel = new QLabel(username);
    usernameLabel->setAlignment(Qt::AlignCenter);
    usernameLabel->setMinimumWidth(200);
    logoutAction = new QAction(tr("登出"), this);
    connect(logoutAction, SIGNAL(triggered()), this, SLOT(logout()));

    settingAction = new QAction(tr("更改设置"), this);
    connect(settingAction, SIGNAL(triggered()), &settingsDialog, SLOT(exec()));

    sortMenu = new QMenu(tr("排序方式"));
    sortBySaveTime = new QAction(tr("按修改时间排序"), this);
    connect(sortBySaveTime, &QAction::triggered, [&](){
       sortJournalsBy(SortBySaveTime);
    });
    sortByRemainder = new QAction(tr("按提醒排序") ,this);
    connect(sortByRemainder, &QAction::triggered, [&](){
       sortJournalsBy(SortByRemainder);
    });
    sortMenu->addAction(sortBySaveTime);
    sortMenu->addAction(sortByRemainder);
    popupBtn = new QToolButton();
    popupBtn->setText(tr("排序方式"));
    popupBtn->setMenu(sortMenu);
    popupBtn->setPopupMode(QToolButton::InstantPopup);

    newAction = new QAction(tr("创建新日程"), this);
    connect(newAction, SIGNAL(triggered()), this, SLOT(startNewJournal()));

    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText(tr(" 搜索一下"));
    // 清除编辑框上的自动聚焦
    searchEdit->clearFocus();
    searchEdit->setStyleSheet("\
            QLineEdit { \
                border: 0;\
            }");
    clearSearchResultAction = new QAction(tr("清空结果"), this);
    clearSearchResultAction->setEnabled(false);
    connect(searchEdit, SIGNAL(textEdited(const QString &)), 
            this, SLOT(enableClearSearch(const QString &)));
    connect(searchEdit, SIGNAL(returnPressed()),
            this, SLOT(startSearching()));
    connect(clearSearchResultAction, SIGNAL(triggered()), 
            this, SLOT(switchSearchToDisplay()));
    connect(clearSearchResultAction, &QAction::triggered, [&]() {
            clearSearchResultAction->setEnabled(false);
    });

    QLabel *rightMargin = new QLabel("");
    rightMargin->setMinimumWidth(50);
    updateAction = new QAction(tr("同步"), this);
    connect(updateAction, SIGNAL(triggered()), this, SLOT(updateJournals()));
    QString lastUpdateTime(tr("尚未同步"));
    settings.value("lastUpdateTime", lastUpdateTime);
    lastUpdateLabel = new QLabel(lastUpdateTime);
    lastUpdateLabel->setMinimumWidth(200);
    lastUpdateLabel->setAlignment(Qt::AlignCenter);
    

    toolbar->addWidget(usernameLabel);
    toolbar->addSeparator();
    toolbar->addAction(logoutAction);
    toolbar->addSeparator();
    toolbar->addAction(settingAction);
    toolbar->addSeparator();
    toolbar->addWidget(popupBtn);
    toolbar->addSeparator();
    toolbar->addAction(newAction);
    toolbar->addSeparator();
    toolbar->addWidget(searchEdit);
    toolbar->addAction(clearSearchResultAction);

    toolbar->addWidget(rightMargin);
    toolbar->addSeparator();
    toolbar->addAction(updateAction);
    toolbar->addSeparator();
    toolbar->addWidget(lastUpdateLabel);
    toolbar->addSeparator();

    mainLayout->addWidget(journalListView, 0, 0);
    mainLayout->addWidget(editJournalPanel, 0, 1);
    
}

void MainWindow::setUpJournals()
{
    editingRow = 0;
    journalListView->setModel(&journalList);
    journalListView->setWordWrap(true);
    journalListView->setSpacing(1);
    connect(journalListView, SIGNAL(clicked(const QModelIndex&)), 
            this, SLOT(getJournalWithIndex(const QModelIndex&)));

    sortJournalsBy(SortBySaveTime);
}

void MainWindow::updateFailed(const QString &msg) const
{
    QMessageBox::information(nullptr, tr("同步失败"), tr("无法获取云端日程\n%1").arg(msg));
    updateAction->setText(tr("同步"));
}

void MainWindow::updateJournals()
{
    Net* net = Net::getNetManager();
    updateAction->setText(tr("开始同步"));
    qWarning() << "before update: total local journals: " <<
                  totalLocalJournals.size();

    QList<BasicJournal> journals;
    if (!(net->getBasicJournalList(journals))) {
        updateFailed();
        return;
    }
    qWarning() << "update: Get BasicJournal " << journals.size();

    if (journals.size() > 1) {
        std::sort(journals.begin(), journals.end());
    }
    if (totalLocalJournals.size() > 1) {
        std::sort(totalLocalJournals.begin(), totalLocalJournals.end());
    }

    QList<QString> willGetObjectIds;
    QList<QString> willMergeObjectIds;
    QMap<QString, LocalJournal> shouldGet;
    QList<LocalJournal> shouldDelete;
    QMap<QString, LocalJournal> shouldMerge;
    QList<BasicJournal> willPostB;
    QList<BasicJournal> willPutB;
    QList<DetailJournal> willPostD;
    QList<DetailJournal> willPutD;
    QList<DetailJournal> willGet;
    QMap<QString, DetailJournal> willMerge;

    auto j = totalLocalJournals.begin();
    auto i = journals.begin();
    // don't modify *j, modify its copy and store to shouldDelete
    while (i != journals.end() || j != totalLocalJournals.end()) {
        // remote journal later than local journal =>
        // should upload these journals to remote
        if (i == journals.end() || (*j) < (*i)) {
            if (!((*j).deleted)) {
                BasicJournal b(*j);
                willPostB.push_back(b);
                DetailJournal d(*j);
                willPostD.push_back(d);
            }
            ++j;
        }
        // remote journal earlier than local journal => 
        // should download these journals to local
        else if (j == totalLocalJournals.end() || (*i) < (*j)) {
            if (!((*i).deleted)) {
                LocalJournal journal;
                journal.deleted = false;
                journal.saveTime = (*i).saveTime;
                journal.journalId = (*i).journalId;
                journal.userName = (*i).username;
                shouldGet[(*i).journalId] = journal;
                willGetObjectIds.push_back((*i).detailObjectId);
            }
            ++i;
        }
        // the remote journal and local journal have same journalId
        else {
            if ((*i).deleted) {
                LocalJournal tmp(*j);
                tmp.clear();
                shouldDelete.push_back(tmp);
            }
            else if ((*j).deleted) {
                (*i).deleteSelf();
                willPutB.push_back(*i);
            }
            else {
                if ((*i).saveTime.isValid() && (*j).saveTime.isValid()) {
                    // there is change in local journal
                    if ((*i).saveTime < (*j).saveTime) {
                        BasicJournal b(*j);
                        willPutB.push_back(b);
                        DetailJournal d(*j);
                        willPutD.push_back(d);
                    }
                    // there is change in remote journal
                    else if ((*i).saveTime > (*j).saveTime){
                        LocalJournal tmp(*j);
                        tmp.saveTime = (*i).saveTime;
                        shouldMerge[tmp.journalId] = tmp;
                        willMerge[tmp.journalId] = DetailJournal();
                        willMergeObjectIds.push_back((*i).detailObjectId);
                    }
                }
            }
            ++i;
            ++j;
        }
    }

    qWarning() << "update: Put BasicJournal " << willPutB.size();
    if (!(net->updateBasicJournal(willPutB))) {
        updateFailed();
        return;
    }
    qWarning() << "update: Put DetailJournal " << willPutD.size();
    if (!(net->updateDetailJournal(willPutD))) {
        updateFailed();
        return;
    }
    qWarning() << "update: Post Journal " << willPostB.size();
    if (!(net->updateRemoteJournal(willPostB, willPostD))) {
        updateFailed();
        return;
    }

    qWarning() << "update: Will Get DetailJournal " << willGetObjectIds.size();
    if (!(net->getDetailJournal(willGetObjectIds, willGet))) {
        updateFailed();
        return;
    }
    qWarning() << "update: Get DetailJournal " << willGet.size();
    if (willGet.size() != willGetObjectIds.size()) {
        updateFailed();
        return;
    }
    for (auto gotJournal : willGet) {
        shouldGet[gotJournal.journalId].detail = gotJournal.detail;
        if (gotJournal.reminder.isValid()) {
            shouldGet[gotJournal.journalId].willAlarm = true;
            shouldGet[gotJournal.journalId].alarmTime = gotJournal.reminder;
        }
        else {
            shouldGet[gotJournal.journalId].willAlarm = false;
        }
    }

    qWarning() << "update: Will Merge DetailJournal " << willMergeObjectIds.size();
    if (!(net->mergeDetailJournal(willMergeObjectIds, willMerge))) {
        updateFailed();
        return;
    }
    qWarning() << "update: Merge DetailJournal " << willMerge.size();
    if (willMerge.size() != willMergeObjectIds.size()) {
        updateFailed();
        return;
    }
    for (auto mergedJournal : shouldMerge) {
        mergedJournal.userName = willMerge[mergedJournal.journalId].username;
        mergedJournal.detail = willMerge[mergedJournal.journalId].detail;
        if (willMerge[mergedJournal.journalId].reminder.isValid()) {
            mergedJournal.willAlarm = true;
            mergedJournal.alarmTime = willMerge[mergedJournal.journalId].reminder;
        }
        else {
            mergedJournal.willAlarm = false;
        }
    }

    updateAction->setText(tr("同步"));
    flushLocalChangeToDB(shouldDelete);
    flushLocalChangeToDB(shouldGet);
    flushLocalChangeToDB(shouldMerge);
    sortJournalsBy(sortBy);
    refreshLastUpdateTime();
}

bool MainWindow::flushLocalChangeToDB(const QList<LocalJournal>& shouldDelete)
{
    (void)shouldDelete;
    return false;
}

bool MainWindow::flushLocalChangeToDB(const QMap<QString, LocalJournal>& 
        shouldMerge)
{
    (void)shouldMerge;
    return false;
}

void MainWindow::refreshLastUpdateTime()
{
    qWarning() << "before lastupdatetime: " << settings.value("lastupdatetime").toString();
    lastUpdateTime = QDateTime::currentDateTime();
    QString lastupdatetime = lastUpdateTime.toString("yyyy年MM月dd日hh时mm分");
    lastUpdateLabel->setText(lastupdatetime);
    settings.setValue("lastupdatetime", lastupdatetime);
    qWarning() << "after lastupdatetime: " << settings.value("lastupdatetime").toString();
}

void MainWindow::startSearching()
{
    isSearched = true;
    // start searching
    QString query = searchEdit->text().simplified();
    JournalsTable *table = JournalsTable::getJournalsTable();
    journalList.setJournals(table->searchJournal(query));
    journalListView->reset();
    if (journalList.size() > 0) {
        editJournalPanel->editLocalJournal(journalList.at(0));
    }
    else {
        editJournalPanel->initEditState();
    }
}

void MainWindow::enableClearSearch(const QString& text)
{
    if (text.size()) {
        clearSearchResultAction->setEnabled(true);
    }
    else {
        clearSearchResultAction->setEnabled(false);
    }
}

// 每次点击时，都应该修改editingRow参数。
void MainWindow::getJournalWithIndex(const QModelIndex& index)
{
    if (!index.isValid()) {
        return;
    }
    editingRow = index.row();
    editJournalPanel->editLocalJournal(journalList.at(editingRow));
}

void MainWindow::startNewJournal()
{
    switchSearchToDisplay();
    editJournalPanel->startNewJournal();
}

void MainWindow::switchSearchToDisplay()
{
    if (isSearched == true) {
        isSearched = false;
        searchEdit->clear();
        clearSearchResultAction->setEnabled(false);
        journalList.setJournals(totalLocalJournals);
        journalListView->reset();
    }
}

void MainWindow::connectEditJournalPanel()
{
    connect(editJournalPanel, SIGNAL(saveLocalJournal(LocalJournal)),
                this, SLOT(saveLocalJournal(LocalJournal)));
    connect(editJournalPanel, SIGNAL(createLocalJournal(LocalJournal&)),
            this, SLOT(createLocalJournal(LocalJournal&)));
    connect(editJournalPanel, SIGNAL(deleteLocalJournal(QString)),
            this, SLOT(deleteLocalJournal(QString)));
}

void MainWindow::sortJournalsBy(SortBy sortBy)
{
    switchSearchToDisplay();
    if (sortBy == this->sortBy)
        return;
    switch (sortBy) {
    case SortBySaveTime:
        popupBtn->setText(tr("按修改时间排序"));
        break;
    case SortByRemainder:
        popupBtn->setText(tr("按提醒排序"));
        break;
    default:
        return;
    }
    totalLocalJournals.clear();
    JournalsTable *table = JournalsTable::getJournalsTable();
    totalLocalJournals = table->selectJournal(sortBy);
    journalList.setJournals(totalLocalJournals);
    journalListView->reset();
}

void MainWindow::deleteLocalJournal(const QString& journalId)
{
    qWarning() << "delete localjournal: " << journalId;
    // update where journalId = xxx ...
    JournalsTable* table = JournalsTable::getJournalsTable();
    table->deleteJournal(journalId);
    if (!journalList.removeJournalWithID(journalId)) {
        qWarning() << "can not delete journalId: " << journalId;
    }
    journalListView->reset();
}
 
void MainWindow::saveLocalJournal(const LocalJournal& journal)
{
    logSaveLocalJournal(journal);
    // update where journalId = xxx.journalId
    JournalsTable* table = JournalsTable::getJournalsTable();
    table->updateJournal(journal);
    if (!journalList.updateJournal(journal)) {
        qWarning() << "can not save journalId: " << journal.journalId;
    }
    journalListView->reset();
}

void MainWindow::createLocalJournal(LocalJournal& journal)
{
    editingRow = 0;
    logCreateLocalJournal(journal);
    // insert with ...
    JournalsTable* table = JournalsTable::getJournalsTable();
    table->insertJournal(journal);
    journal.userName = username;
    if (!journalList.addJournal(journal)) {
        qWarning() << "can not create journalId: " << journal.journalId;
    }
    journalListView->reset();
}

void MainWindow::logout()
{
    this->hide();
    LoginDialog loginDialog;
    loginDialog.bindMainwindow(this);
    if (loginDialog.exec() == QDialog::Accepted) {
        usernameLabel->setText(username);
        searchEdit->clear();
        setUpJournals();
        const bool selected = settings.value("autoupdate", true).toBool();
        if (selected) {
            updateJournals();
        }
        showMaximized();
    }
    else {
        show();
    }
}


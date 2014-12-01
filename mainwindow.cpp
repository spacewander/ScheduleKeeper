#include <QDebug>
#include <QIcon>
#include <QToolButton>
#include <QtSql>

#include "mainwindow.h"
#include "editjournalpanel.h"
#include "global.h"
#include "logindialog.h"

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

    qDebug() << "username: " << username;
    usernameLabel = new QLabel(username);
    usernameLabel->setAlignment(Qt::AlignCenter);
    usernameLabel->setMinimumWidth(200);
    logoutAction = new QAction(tr("登出"), this);
    connect(logoutAction, SIGNAL(triggered()), this, SLOT(logout()));

    settingAction = new QAction(tr("更改设置"), this);
    connect(settingAction, SIGNAL(triggered()), &settingsDialog, SLOT(exec()));

    sortMenu = new QMenu(tr("排序方式"));
    sortByCreatedTime = new QAction(tr("按创建时间排序"), this);
    sortBySaveTime = new QAction(tr("按修改时间排序"), this);
    sortByRemainder = new QAction(tr("按提醒排序") ,this);
    sortMenu->addAction(sortByCreatedTime);
    sortMenu->addAction(sortBySaveTime);
    sortMenu->addAction(sortByRemainder);
    QToolButton* popupBtn = new QToolButton();
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

    totalLocalJournals.clear();
    // test data begin
//    QString testDetail = "生活就像海洋，只有意志坚强的人才能到达彼岸。";
//    LocalJournal test1("111111", QDateTime(QDate(2014, 7, 28), QTime(23, 10)),
//                       QDateTime(QDate(2014, 11, 27), QTime(22, 18)), testDetail);
//    QString test2Detail = "Ent_evo #imaginature# 一个世纪过去了，蚯蚓的王国又向大地的尽头前进了一公里。它们不急。从来都不急。";
//    LocalJournal test2("111112", QDateTime(QDate(2014, 10, 28), QTime(3, 10)),
//                       QDateTime(QDate(2014, 11, 27), QTime(12, 48)), test2Detail,
//                       QDateTime(QDate(2014, 12, 1), QTime(12, 0)));
//    totalLocalJournals.push_back(test1);
//    totalLocalJournals.push_back(test2);
    // test data end
    journalList.setJournals(totalLocalJournals);
}

void MainWindow::updateJournals()
{
}

void MainWindow::refreshLastUpdateTime()
{
    qDebug() << "before lastupdatetime: " << settings.value("lastupdatetime").toString();
    lastUpdateTime = QDateTime::currentDateTime();
    QString lastupdatetime = lastUpdateTime.toString("yyyy.MM.dd.hh.mm");
    lastUpdateLabel->setText(lastupdatetime);
    settings.setValue("lastupdatetime", lastupdatetime);
    qDebug() << "after lastupdatetime: " << settings.value("lastupdatetime").toString();
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
    isSearched = false;
    searchEdit->clear();
    clearSearchResultAction->setEnabled(false);
}

void MainWindow::connectEditJournalPanel()
{
    connect(editJournalPanel, SIGNAL(saveLocalJournal(LocalJournal)),
                this, SLOT(saveLocalJournal(LocalJournal)));
    connect(editJournalPanel, SIGNAL(createLocalJournal(LocalJournal)),
            this, SLOT(createLocalJournal(LocalJournal)));
    connect(editJournalPanel, SIGNAL(deleteLocalJournal(QString)),
            this, SLOT(deleteLocalJournal(QString)));
}

void MainWindow::deleteLocalJournal(const QString& journalID)
{
    qDebug() << "delete localjournal: " << journalID;
    // update where journalID = xxx ...
    if (!journalList.removeJournalWithID(journalID)) {
        qDebug() << "can not delete journalID: " << journalID;
    }
    journalListView->reset();
}
 
void MainWindow::saveLocalJournal(const LocalJournal& journal)
{
    logSaveLocalJournal(journal);
    // update where journalID = xxx.journalID
    if (!journalList.updateJournal(journal)) {
        qDebug() << "can not save journalID: " << journal.journalID;
    }
    journalListView->reset();
}

void MainWindow::createLocalJournal(const LocalJournal& journal)
{
    editingRow = 0;
    logCreateLocalJournal(journal);
    // insert with ...
    if (!journalList.addJournal(journal)) {
        qDebug() << "can not create journalID: " << journal.journalID;
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


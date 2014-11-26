#include <QDebug>
#include <QToolButton>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    settings("ScheduleKeeper", "ScheduleKeeper")
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

}

MainWindow::~MainWindow()
{

}

void MainWindow::setUsername(const QString& username)
{
    this->username = username;
}

void MainWindow::setUpGUI()
{
    // 顶部为工具栏，宽度跟最长的用户名（20个汉字）相对。
    // 从左到右分别为用户名、设置、搜索、排序、添加、更新、更新情况
    // 第一列为日程列表
    // 第二列为日程编辑界面
    mainLayout = new QGridLayout(centralWidget);

    toolbar = addToolBar("");
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setContentsMargins(100, 0, 0, 0);
    toolbar->setCursor(Qt::PointingHandCursor);
    toolbar->setStyleSheet("QToolBar {\
            background: rgba(255,255,255,1);\
            }");

    QLabel *leftMargin = new QLabel("");
    leftMargin->setMinimumWidth(50);
    usernameLabel = new QLabel(username);
    usernameLabel->setMinimumWidth(200);
    settingAction = new QAction(tr("更改设置"), this);

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

    QLabel *rightMargin = new QLabel("");
    rightMargin->setMinimumWidth(50);
    updateAction = new QAction(tr("同步"), this);
    QString lastUpdateTime(tr("尚未同步"));
    settings.value("lastUpdateTime", lastUpdateTime);
    lastUpdateLabel = new QLabel(lastUpdateTime);
    lastUpdateLabel->setMinimumWidth(200);
    lastUpdateLabel->setAlignment(Qt::AlignCenter);
    

    toolbar->addWidget(leftMargin);
    toolbar->addWidget(usernameLabel);
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

    journalListView = new QListView(this);

    mainLayout->addWidget(journalListView, 0, 0);
    
}

void MainWindow::refreshLastUpdateTime()
{

}

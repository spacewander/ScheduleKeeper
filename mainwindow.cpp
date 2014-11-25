#include <QDebug>

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
    // 第一列为工具栏，宽度跟最长的用户名（20个汉字）相对。
    // 从上到下包括了用户名、设置、搜索、排序、添加、更新、更新情况
    // 第二列为日程列表
    // 第三列为日程编辑界面
    mainLayout = new QGridLayout(centralWidget);

    toolbarLayout = new QVBoxLayout();
    usernameLabel = new QLabel(username);
    settingButton = new QPushButton(tr("设置"));
    toolbarLayout->addWidget(usernameLabel);
    toolbarLayout->addWidget(settingButton);

    journalListView = new QListView(this);

    mainLayout->addLayout(toolbarLayout, 0, 0, 7, 1);
    mainLayout->addWidget(journalListView, 0, 1);
    
}

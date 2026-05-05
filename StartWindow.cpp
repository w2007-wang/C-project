#include "StartWindow.h"

StartWindow::StartWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("人偶对决");
    resize(800, 600); // 初始大小，可随意改

    // 标题
    QLabel *title = new QLabel("人偶对决", this);
    QFont font = title->font();
    font.setPointSize(40);
    font.setBold(true);
    title->setFont(font);
    title->setAlignment(Qt::AlignCenter);

    // 按钮
    QPushButton *btnStart = new QPushButton("开始游戏", this);
    QPushButton *btnExit  = new QPushButton("退出游戏", this);

    // 按钮样式与最小尺寸
    btnStart->setMinimumSize(200, 60);
    btnExit->setMinimumSize(200, 60);

    // 布局自动居中、自适应窗口大小
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch(1);
    layout->addWidget(title);
    layout->addSpacing(50);
    layout->addWidget(btnStart, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(btnExit, 0, Qt::AlignCenter);
    layout->addStretch(2);

    setLayout(layout);

    connect(btnStart, &QPushButton::clicked, this, &StartWindow::onStartClicked);
    connect(btnExit,  &QPushButton::clicked, this, &StartWindow::onExitClicked);
}

void StartWindow::onStartClicked()
{
    emit startGameClicked();
    close();
}

void StartWindow::onExitClicked()
{
    close();
}

#include "StartWindow.h"
#include <QPixmap>
#include <QPainter>
#include <QGuiApplication>
#include <QScreen>

StartWindow::StartWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("人偶对决");
    
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    int w = screenRect.width() * 3 / 4;
    int h = screenRect.height() * 3 / 4;
    resize(w, h);

    m_background.load(":/background.npg.jpg"); // 从资源文件加载背景图片

    // 标题
    QLabel *title = new QLabel("人偶对决", this);
    QFont font = title->font();
    font.setPointSize(40);
    font.setBold(true);
    title->setFont(font);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: white; text-shadow: 2px 2px 4px black;");

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

void StartWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if (!m_background.isNull()) {
        painter.drawPixmap(rect(), m_background.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
    QWidget::paintEvent(event);
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

#include "ResultWindow.h"
#include <QPainter>
#include <QPixmap>
#include <QGuiApplication>
#include <QScreen>
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <QHBoxLayout>

ResultWindow::ResultWindow(int winner, QWidget *parent)
    : QWidget(parent), m_winner(winner)
{
    setWindowTitle("游戏结束");

    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    int w = screenRect.width() * 3 / 4;
    int h = screenRect.height() * 3 / 4;
    resize(w, h);

    QLabel *resultLabel = new QLabel(this);
    resultLabel->setText(m_winner == 1 ? "玩家一胜利！" : "玩家二胜利！");
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setStyleSheet(
        "color: #FFD700;"
        "font-size: 64px;"
        "font-weight: bold;"
        "font-family: 'Microsoft YaHei';"
        "background: transparent;"
        "padding: 20px;"
    );

    QGraphicsDropShadowEffect *textShadow = new QGraphicsDropShadowEffect(this);
    textShadow->setBlurRadius(25);
    textShadow->setColor(QColor(0, 0, 0, 180));
    textShadow->setOffset(4, 4);
    resultLabel->setGraphicsEffect(textShadow);

    QString btnStyle =
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 rgba(80,80,80,240), stop:1 rgba(50,50,50,240));"
        "  color: white;"
        "  font-size: 22px;"
        "  font-weight: bold;"
        "  font-family: 'Microsoft YaHei';"
        "  border: 2px solid rgba(100,100,100,255);"
        "  border-radius: 14px;"
        "  padding: 16px 32px;"
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 rgba(100,100,100,240), stop:1 rgba(70,70,70,240));"
        "  border: 2px solid rgba(150,150,150,200);"
        "}"
        "QPushButton:pressed {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 rgba(40,40,40,240), stop:1 rgba(30,30,30,240));"
        "}";

    QPushButton *btnRestart = new QPushButton("重新开始", this);
    QPushButton *btnHome = new QPushButton("返回主页", this);

    btnRestart->setStyleSheet(btnStyle);
    btnHome->setStyleSheet(btnStyle);

    btnRestart->setCursor(Qt::PointingHandCursor);
    btnHome->setCursor(Qt::PointingHandCursor);

    btnRestart->setFixedSize(220, 70);
    btnHome->setFixedSize(220, 70);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(btnRestart);
    btnLayout->addSpacing(40);
    btnLayout->addWidget(btnHome);
    btnLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch(3);
    mainLayout->addWidget(resultLabel);
    mainLayout->addStretch(2);
    mainLayout->addLayout(btnLayout);
    mainLayout->addStretch(1);

    setLayout(mainLayout);

    connect(btnRestart, &QPushButton::clicked, this, [this]() {
        emit restartGame();
        close();
    });
    connect(btnHome, &QPushButton::clicked, this, [this]() {
        emit returnToStart();
        close();
    });
}

void ResultWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    QPixmap bg(":/背景3.jpg");
    if (!bg.isNull()) {
        painter.drawPixmap(rect(), bg.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }
    painter.fillRect(rect(), QColor(255, 255, 255, 80));

    painter.fillRect(rect(), QColor(0, 0, 0, 100));

    QWidget::paintEvent(event);
}
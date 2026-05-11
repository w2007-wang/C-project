#include "StartWindow.h"
#include <QPixmap>
#include <QPainter>
#include <QGuiApplication>
#include <QScreen>
#include <QGraphicsDropShadowEffect>

StartWindow::StartWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("人偶对决");
    
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    int w = screenRect.width() * 3 / 4;
    int h = screenRect.height() * 3 / 4;
    resize(w, h);

    m_background.load(":/background.npg.jpg");

    QLabel *title = new QLabel(this);
    QPixmap titlePixmap(":/标题.jpg");
    if (!titlePixmap.isNull()) {
        QImage img = titlePixmap.toImage().convertToFormat(QImage::Format_ARGB32);
        for (int y = 0; y < img.height(); ++y) {
            QRgb *row = reinterpret_cast<QRgb*>(img.scanLine(y));
            for (int x = 0; x < img.width(); ++x) {
                int r = qRed(row[x]);
                int g = qGreen(row[x]);
                int b = qBlue(row[x]);
                if (r > 230 && g > 230 && b > 230) {
                    row[x] = qRgba(0, 0, 0, 0);
                } else {
                    int alpha = 200;
                    row[x] = qRgba(r, g, b, alpha);
                }
            }
        }
        QPixmap transparentPixmap = QPixmap::fromImage(img);
        int titleWidth = w * 0.45;
        int titleHeight = titleWidth * transparentPixmap.height() / transparentPixmap.width();
        title->setPixmap(transparentPixmap.scaled(titleWidth, titleHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("background: transparent;");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(0, 0, 0, 150));
    shadow->setOffset(2, 2);
    title->setGraphicsEffect(shadow);

    QPushButton *btnStart = new QPushButton("开始游戏", this);
    QPushButton *btnExit  = new QPushButton("退出游戏", this);

    btnStart->setFixedSize(240, 70);
    btnExit->setFixedSize(240, 70);

    QString btnStyle =
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 rgba(80,80,80,240), stop:1 rgba(50,50,50,240));"
        "  color: white;"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "  border: 2px solid rgba(100,100,100,255);"
        "  border-radius: 12px;"
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

    btnStart->setStyleSheet(btnStyle);
    btnExit->setStyleSheet(btnStyle);

    btnStart->setCursor(Qt::PointingHandCursor);
    btnExit->setCursor(Qt::PointingHandCursor);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch(2);
    layout->addWidget(title);
    layout->addSpacing(60);
    layout->addWidget(btnStart, 0, Qt::AlignCenter);
    layout->addSpacing(24);
    layout->addWidget(btnExit, 0, Qt::AlignCenter);
    layout->addStretch(3);

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
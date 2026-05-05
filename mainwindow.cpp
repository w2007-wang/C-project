#include "mainwindow.h"
#include <QMessageBox>
#include <QFont>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 不再固定大小！允许自由缩放
    resize(800, 900);
    setWindowTitle("箭头人偶对决 - 可缩放窗口");

    m_boardMargin = 80;

    board = new GameBoard(this);
    doll1 = new Doll(1, this);
    doll2 = new Doll(2, this);

    turnTimer = new QTimer(this);
    turnTimer->start(1000);
    connect(turnTimer, &QTimer::timeout, this, &MainWindow::updateTurnTimer);

    moveTimer = new QTimer(this);
    moveTimer->setInterval(180);
    connect(moveTimer, &QTimer::timeout, this, &MainWindow::updateMoveAnimation);

    currPlayer = 1;
    turnTime = 60;
    gameEnd = false;
    currDoll = doll1;
}

MainWindow::~MainWindow()
{
}

// 窗口大小变化时自动重绘
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    update();
}

// 计算棋盘整体矩形
QRect MainWindow::getBoardRect() const
{
    int w = width();
    int h = height() - m_boardMargin;

    int boardPixel = qMin(w, h);
    int x = (w - boardPixel) / 2;
    int y = m_boardMargin / 2;

    return QRect(x, y, boardPixel, boardPixel);
}

int MainWindow::getCellSize() const
{
    return getBoardRect().width() / 9;
}

// 网格坐标 → 屏幕坐标
QPoint MainWindow::gridToPos(int x, int y) const
{
    QRect br = getBoardRect();
    int cs = getCellSize();
    return QPoint(br.x() + x * cs + cs/2, br.y() + y * cs + cs/2);
}

// 屏幕坐标 → 网格坐标
QPoint MainWindow::posToGrid(const QPoint &pos) const
{
    QRect br = getBoardRect();
    int cs = getCellSize();
    if (cs <= 0) return QPoint(0,0);

    int gx = (pos.x() - br.x()) / cs;
    int gy = (pos.y() - br.y()) / cs;

    gx = qBound(0, gx, 8);
    gy = qBound(0, gy, 8);
    return QPoint(gx, gy);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    if (gameEnd) {
        p.setFont(QFont("Arial", 30));
        p.setPen(Qt::white);  // 游戏结束文字白色
        p.drawText(rect(), Qt::AlignCenter, "游戏结束");
        return;
    }

    drawBoard(p);
    drawDoll(p, doll1, Qt::red);
    drawDoll(p, doll2, Qt::blue);
    drawHP(p);

    // ========== 这里改成白色 ==========
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 11));
    p.drawText(10, 25,
               QString("玩家%1 | 时间:%2秒 | WASD选方向 | 空格发射")
                   .arg(currPlayer).arg(turnTime));
}

void MainWindow::drawBoard(QPainter &p)
{
    QRect br = getBoardRect();
    int cs = getCellSize();

    for (int y = 0; y < 9; ++y) {
        for (int x = 0; x < 9; ++x) {
            QRect r(br.x() + x*cs, br.y() + y*cs, cs, cs);
            p.setPen(Qt::black);
            p.setBrush((x + y) % 2 ? Qt::white : QColor(240,240,240));
            p.drawRect(r);

            Cell c = board->getCell(x, y);
            if (c.isBlock) {
                p.setBrush(Qt::darkGray);
                p.drawRect(r.adjusted(4,4,-4,-4));
            }
            if (c.hasArrow) {
                p.setPen(Qt::blue);
                QPoint ct = r.center();
                int len = cs * 0.25;
                switch (c.dir) {
                case Right: p.drawLine(ct, ct + QPoint(len,0)); break;
                case Down:  p.drawLine(ct, ct + QPoint(0,len)); break;
                case Left:  p.drawLine(ct, ct - QPoint(len,0)); break;
                case Up:    p.drawLine(ct, ct - QPoint(0,len)); break;
                default: break;
                }
            }
        }
    }
}

void MainWindow::drawDoll(QPainter &p, Doll *d, const QColor &color)
{
    QPoint pt = gridToPos(d->x, d->y);
    int cs = getCellSize();
    int r = cs * 0.2;

    p.setBrush(color);
    p.drawEllipse(pt, r, r);
}

void MainWindow::drawHP(QPainter &p)
{
    int w = width();
    int barW = w * 0.3;
    int barH = 18;

    // 玩家1
    int w1 = barW * doll1->hp / 100;
    p.setBrush(Qt::lightGray);
    p.drawRect(20, height()-40, barW, barH);
    p.setBrush(Qt::red);
    p.drawRect(20, height()-40, w1, barH);
    p.setPen(Qt::white); // 血量文字白色
    p.drawText(20, height()-15, QString("玩家1 HP: %1").arg(doll1->hp));

    // 玩家2
    int w2 = barW * doll2->hp / 100;
    int px2 = w - barW - 20;
    p.setBrush(Qt::lightGray);
    p.drawRect(px2, height()-40, barW, barH);
    p.setBrush(Qt::blue);
    p.drawRect(px2, height()-40, w2, barH);
    p.setPen(Qt::white); // 血量文字白色
    p.drawText(px2, height()-15, QString("玩家2 HP: %1").arg(doll2->hp));
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (gameEnd || moveTimer->isActive()) return;
    QPoint g = posToGrid(e->pos());
    board->rotateArrow(g.x(), g.y());
    update();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (gameEnd) return;

    if (e->key() == Qt::Key_W && !moveTimer->isActive())
        currDoll->setLaunchDirection(Up);
    if (e->key() == Qt::Key_S && !moveTimer->isActive())
        currDoll->setLaunchDirection(Down);
    if (e->key() == Qt::Key_A && !moveTimer->isActive())
        currDoll->setLaunchDirection(Left);
    if (e->key() == Qt::Key_D && !moveTimer->isActive())
        currDoll->setLaunchDirection(Right);

    if (e->key() == Qt::Key_Space && !moveTimer->isActive())
        launch();

    update();
}

void MainWindow::launch()
{
    currDoll->arrowCount = 0;
    currDoll->applyLaunchDirection();
    currDoll->isMoving = true;
    moveTimer->start();
}

void MainWindow::updateMoveAnimation()
{
    int oldX = currDoll->x;
    int oldY = currDoll->y;
    currDoll->moveStep();
    int x = currDoll->x;
    int y = currDoll->y;

    if (x < 0 || x >= 9 || y < 0 || y >= 9) {
        currDoll->x = oldX;
        currDoll->y = oldY;
        currDoll->isMoving = false;
        moveTimer->stop();
        switchPlayer();
        return;
    }

    Cell cell = board->getCell(x, y);
    Doll* enemy = (currPlayer == 1) ? doll2 : doll1;

    if (cell.isBlock) {
        currDoll->x = oldX;
        currDoll->y = oldY;
        currDoll->isMoving = false;
        moveTimer->stop();
        switchPlayer();
        return;
    }

    if (x == enemy->x && y == enemy->y) {
        currDoll->x = oldX;
        currDoll->y = oldY;

        int damage = currDoll->arrowCount * 2;
        enemy->hurt(damage);

        QMessageBox::information(this, "命中",
                                 QString("途经箭头:%1 → 造成伤害:%2")
                                     .arg(currDoll->arrowCount).arg(damage));

        if (doll1->isDead()) gameOver(2);
        if (doll2->isDead()) gameOver(1);

        currDoll->isMoving = false;
        moveTimer->stop();
        switchPlayer();
        return;
    }

    if (cell.hasArrow) {
        currDoll->currentDir = cell.dir;
        currDoll->arrowCount++;
    }

    update();
}

void MainWindow::updateTurnTimer()
{
    if (gameEnd || moveTimer->isActive()) return;
    if (--turnTime <= 0)
        switchPlayer();
    update();
}

void MainWindow::switchPlayer()
{
    currPlayer = (currPlayer == 1) ? 2 : 1;
    turnTime = 60;
    currDoll = (currPlayer == 1) ? doll1 : doll2;
    update();
}

void MainWindow::gameOver(int winner)
{
    gameEnd = true;
    QMessageBox::information(this, "游戏结束", QString("玩家%1 胜利！").arg(winner));
    QMessageBox::information(this, "重置", "确定后重新开始");

    doll1->reset();
    doll2->reset();
    board->initBoard();
    gameEnd = false;
    currPlayer = 1;
    turnTime = 60;
    update();
}

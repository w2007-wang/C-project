#include "mainwindow.h"
#include <QMessageBox>
#include <QFont>
#include <QResizeEvent>
#include <QGuiApplication>
#include <QScreen>
#include <QRadialGradient>
#include <QVBoxLayout>
#include <QLabel>
#include <QRandomGenerator>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    int w = screenRect.width() * 3 / 4;
    int h = screenRect.height() * 3 / 4;
    resize(w, h);
    setWindowTitle("箭头人偶对决 - 可缩放窗口");

    board = std::make_unique<GameBoard>();
    doll1 = std::make_unique<Doll>(1);
    doll2 = std::make_unique<Doll>(2);

    turnTimer = new QTimer(this);
    turnTimer->start(1000);
    connect(turnTimer, &QTimer::timeout, this, &MainWindow::updateTurnTimer);

    moveTimer = new QTimer(this);
    moveTimer->setInterval(MOVE_INTERVAL);
    connect(moveTimer, &QTimer::timeout, this, &MainWindow::updateMoveAnimation);

    animTimer = new QTimer(this);
    animTimer->setInterval(ANIM_INTERVAL);
    connect(animTimer, &QTimer::timeout, this, &MainWindow::updateAnimFrame);

    pauseButton = new QPushButton("暂停", this);
    pauseButton->setFocusPolicy(Qt::NoFocus);
    pauseButton->setFixedSize(80, 36);
    pauseButton->setStyleSheet(
        "QPushButton { background-color: rgba(0,0,0,180); color: white; "
        "border: 1px solid #888; border-radius: 6px; font-size: 14px; }"
        "QPushButton:hover { background-color: rgba(60,60,60,200); }"
    );
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseGame);

    loadP1Sprites();
    loadP2Sprites();
    currDoll = doll1.get();
}

MainWindow::~MainWindow() = default;

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    pauseButton->move(width() - pauseButton->width() - 20, 12);
    update();
}

QRect MainWindow::getBoardRect() const {
    int w = width();
    int h = height() - BOARD_MARGIN;
    int boardPixel = qMin(w, h);
    int x = (w - boardPixel) / 2;
    int y = BOARD_MARGIN / 2;
    return QRect(x, y, boardPixel, boardPixel);
}

int MainWindow::getCellSize() const {
    return getBoardRect().width() / GameBoard::SIZE;
}

QPoint MainWindow::gridToPos(int x, int y) const {
    QRect br = getBoardRect();
    int cs = getCellSize();
    return QPoint(br.x() + x * cs + cs / 2, br.y() + y * cs + cs / 2);
}

QPointF MainWindow::gridToPosF(float x, float y) const {
    QRect br = getBoardRect();
    int cs = getCellSize();
    return QPointF(br.x() + x * cs + cs / 2.0, br.y() + y * cs + cs / 2.0);
}

QPoint MainWindow::posToGrid(const QPoint &pos) const {
    QRect br = getBoardRect();
    int cs = getCellSize();
    if (cs <= 0) return QPoint(0, 0);

    int gx = (pos.x() - br.x()) / cs;
    int gy = (pos.y() - br.y()) / cs;

    gx = qBound(0, gx, GameBoard::SIZE - 1);
    gy = qBound(0, gy, GameBoard::SIZE - 1);
    return QPoint(gx, gy);
}

bool MainWindow::isOutOfBounds(int x, int y) const {
    return x < 0 || x >= GameBoard::SIZE || y < 0 || y >= GameBoard::SIZE;
}

Doll* MainWindow::getEnemy() const {
    return (currPlayer == 1) ? doll2.get() : doll1.get();
}

void MainWindow::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    if (gameEnd) {
        p.setFont(QFont("Arial", 30));
        p.setPen(Qt::white);
        p.drawText(rect(), Qt::AlignCenter, "游戏结束");
        return;
    }

    drawBoard(p);
    drawDoll(p, doll1.get(), Qt::red);
    drawDoll(p, doll2.get(), Qt::blue);
    drawHP(p);

    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 11));
    p.drawText(10, 25, QString("玩家%1 | 时间:%2秒 | WASD选方向 | 空格发射")
                   .arg(currPlayer).arg(turnTime));
}

void MainWindow::drawBoard(QPainter &p) {
    QRect br = getBoardRect();
    int cs = getCellSize();

    for (int y = 0; y < GameBoard::SIZE; ++y) {
        for (int x = 0; x < GameBoard::SIZE; ++x) {
            QRect r(br.x() + x * cs, br.y() + y * cs, cs, cs);
            p.setPen(Qt::black);
            p.setBrush(Qt::white);
            p.drawRect(r);

            Cell c = board->getCell(x, y);
            if (c.isBlock) {
                p.setBrush(Qt::darkGray);
                p.drawRect(r.adjusted(4, 4, -4, -4));
            }
            if (c.hasArrow) {
                drawArrow(p, r, c.dir, cs);
            }
            if (c.hasChest) {
                drawChest(p, r, cs);
            }
            if (c.hasPortal) {
                drawPortal(p, r, cs, c.portalDir);
            }
            if (c.hasHealthPack) {
                drawHealthPack(p, r, cs);
            }
        }
    }
}

void MainWindow::drawPortal(QPainter &p, const QRect &cellRect, int cellSize, PortalDirection dir) {
    QString imagePath;
    switch (dir) {
        case PortalFront: imagePath = ":/正面.png"; break;
        case PortalLeft:   imagePath = ":/左面.png"; break;
        case PortalBack:   imagePath = ":/背面.png"; break;
        case PortalRight:  imagePath = ":/右面.png"; break;
        default:           imagePath = ":/正面.png";
    }
    
    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        QRect doorRect = cellRect.adjusted(cellSize * 0.1, cellSize * 0.1, -cellSize * 0.1, -cellSize * 0.1);
        p.fillRect(doorRect, QColor(128, 0, 128));
        p.setPen(Qt::white);
        p.drawText(doorRect, Qt::AlignCenter, "门");
        return;
    }
    
    QImage img = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < img.height(); ++y) {
        QRgb *row = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < img.width(); ++x) {
            int r = qRed(row[x]);
            int g = qGreen(row[x]);
            int b = qBlue(row[x]);
            if (r > 230 && g > 230 && b > 230) {
                row[x] = qRgba(0, 0, 0, 0);
            }
        }
    }
    QPixmap transparentPixmap = QPixmap::fromImage(img);
    
    QPixmap scaledPixmap = transparentPixmap.scaled(cellSize * 0.8, cellSize * 0.8, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPoint offset((cellSize - scaledPixmap.width()) / 2, (cellSize - scaledPixmap.height()) / 2);
    p.drawPixmap(cellRect.topLeft() + offset, scaledPixmap);
}

void MainWindow::drawArrow(QPainter &p, const QRect &cellRect, Direction dir, int cellSize) {
    QPoint center = cellRect.center();
    const double arrowLength = cellSize * 0.28;
    const double headLength = cellSize * 0.16;
    const double headWidth = cellSize * 0.10;
    const double lineWidth = cellSize * 0.06;

    QColor arrowColor(0, 0, 0);
    QPen arrowPen(arrowColor, lineWidth);
    arrowPen.setCapStyle(Qt::RoundCap);
    p.setPen(arrowPen);
    p.setBrush(arrowColor);

    QPoint lineStart, lineEnd, arrowTip;

    switch (dir) {
    case Right:
        lineStart = QPoint(center.x() - arrowLength, center.y());
        lineEnd = QPoint(center.x() + arrowLength * 0.3, center.y());
        arrowTip = center + QPoint(arrowLength, 0);
        break;
    case Left:
        lineStart = QPoint(center.x() + arrowLength, center.y());
        lineEnd = QPoint(center.x() - arrowLength * 0.3, center.y());
        arrowTip = center - QPoint(arrowLength, 0);
        break;
    case Down:
        lineStart = QPoint(center.x(), center.y() - arrowLength);
        lineEnd = QPoint(center.x(), center.y() + arrowLength * 0.3);
        arrowTip = center + QPoint(0, arrowLength);
        break;
    case Up:
    default:
        lineStart = QPoint(center.x(), center.y() + arrowLength);
        lineEnd = QPoint(center.x(), center.y() - arrowLength * 0.3);
        arrowTip = center - QPoint(0, arrowLength);
        break;
    }

    p.drawLine(lineStart, lineEnd);

    QPolygon head;
    switch (dir) {
    case Right:
        head << arrowTip << QPoint(arrowTip.x() - headLength, arrowTip.y() - headWidth)
             << QPoint(arrowTip.x() - headLength, arrowTip.y() + headWidth);
        break;
    case Left:
        head << arrowTip << QPoint(arrowTip.x() + headLength, arrowTip.y() - headWidth)
             << QPoint(arrowTip.x() + headLength, arrowTip.y() + headWidth);
        break;
    case Down:
        head << arrowTip << QPoint(arrowTip.x() - headWidth, arrowTip.y() - headLength)
             << QPoint(arrowTip.x() + headWidth, arrowTip.y() - headLength);
        break;
    case Up:
    default:
        head << arrowTip << QPoint(arrowTip.x() - headWidth, arrowTip.y() + headLength)
             << QPoint(arrowTip.x() + headWidth, arrowTip.y() + headLength);
        break;
    }
    p.drawPolygon(head);
}

void MainWindow::drawChest(QPainter &p, const QRect &cellRect, int cellSize) {
    QPoint center = cellRect.center();
    const int chestWidth = cellSize * 0.5;
    const int chestHeight = cellSize * 0.4;
    const int x = center.x() - chestWidth / 2;
    const int y = center.y() - chestHeight / 2;

    QRect chestRect(x, y, chestWidth, chestHeight);

    QColor chestColor(210, 180, 140);
    QColor lidColor(180, 140, 100);
    QColor lockColor(255, 215, 0);

    p.setBrush(chestColor);
    p.setPen(Qt::darkGray);
    p.drawRect(chestRect);

    QRect lidRect(x, y, chestWidth, chestHeight * 0.4);
    p.setBrush(lidColor);
    p.drawRect(lidRect);

    p.setBrush(lockColor);
    p.drawRect(center.x() - 3, center.y() - 2, 6, 8);
}

void MainWindow::drawHealthPack(QPainter &p, const QRect &cellRect, int cellSize) {
    const int radius = cellSize * 0.18;
    const QPoint center = cellRect.center();

    QRadialGradient gradient(center, radius);
    gradient.setColorAt(0, QColor(100, 255, 100));
    gradient.setColorAt(0.7, QColor(30, 180, 30));
    gradient.setColorAt(1, QColor(0, 120, 0));
    p.setPen(QPen(QColor(0, 100, 0), 1.5));
    p.setBrush(gradient);
    p.drawEllipse(center, radius, radius);

    const int crossW = radius * 0.5;
    const int crossH = radius * 0.22;
    p.setBrush(QColor(255, 255, 255, 220));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(QRectF(center.x() - crossH, center.y() - crossW, crossH * 2, crossW * 2), 1, 1);
    p.drawRoundedRect(QRectF(center.x() - crossW, center.y() - crossH, crossW * 2, crossH * 2), 1, 1);
}

void MainWindow::drawDoll(QPainter &p, const Doll *d, const QColor &color) {
    QPointF pt = gridToPosF(d->smoothX, d->smoothY);
    int cs = getCellSize();

    QPixmap sprite;
    if (d->player == 1) {
        sprite = getP1Sprite(d->currentDir, d->animFrame);
    } else {
        sprite = getP2Sprite(d->currentDir, d->animFrame);
    }

    if (!sprite.isNull()) {
        QImage img = sprite.toImage().convertToFormat(QImage::Format_ARGB32);
        for (int y = 0; y < img.height(); ++y) {
            QRgb *row = reinterpret_cast<QRgb*>(img.scanLine(y));
            for (int x = 0; x < img.width(); ++x) {
                int r = qRed(row[x]);
                int g = qGreen(row[x]);
                int b = qBlue(row[x]);
                if (r > 230 && g > 230 && b > 230) {
                    row[x] = qRgba(0, 0, 0, 0);
                }
            }
        }
        QPixmap transparentSprite = QPixmap::fromImage(img);

        QPixmap scaled = transparentSprite.scaled(cs * 0.8, cs * 0.8, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPointF offset((cs - scaled.width()) / 2.0, (cs - scaled.height()) / 2.0);
        p.drawPixmap(QPointF(pt.x() - cs / 2.0, pt.y() - cs / 2.0) + offset, scaled);
    } else {
        int r = cs * 0.2;
        p.setBrush(color);
        p.drawEllipse(pt, r, r);
    }
}

void MainWindow::drawHP(QPainter &p) {
    const int w = width();
    const int barW = w * 0.3;
    const int barH = 18;

    const int w1 = barW * doll1->hp / Doll::INITIAL_HP;
    p.setBrush(Qt::lightGray);
    p.drawRect(20, height() - 40, barW, barH);
    p.setBrush(Qt::red);
    p.drawRect(20, height() - 40, w1, barH);
    p.setPen(Qt::white);
    p.drawText(20, height() - 15, QString("玩家1 HP: %1 | 加成: %2").arg(doll1->hp).arg(doll1->baseDamage));

    const int w2 = barW * doll2->hp / Doll::INITIAL_HP;
    const int px2 = w - barW - 20;
    p.setBrush(Qt::lightGray);
    p.drawRect(px2, height() - 40, barW, barH);
    p.setBrush(Qt::blue);
    p.drawRect(px2, height() - 40, w2, barH);
    p.setPen(Qt::white);
    p.drawText(px2, height() - 15, QString("玩家2 HP: %1 | 加成: %2").arg(doll2->hp).arg(doll2->baseDamage));
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
    if (gameEnd || moveTimer->isActive() || isPaused) return;
    QPoint g = posToGrid(e->pos());
    
    Cell cell = board->getCell(g.x(), g.y());
    if (cell.hasPortal) {
        board->rotatePortal(g.x(), g.y());
    } else {
        board->rotateArrow(g.x(), g.y());
    }
    update();
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (gameEnd || moveTimer->isActive() || isPaused) return;

    switch (e->key()) {
    case Qt::Key_W: currDoll->setLaunchDirection(Up); break;
    case Qt::Key_S: currDoll->setLaunchDirection(Down); break;
    case Qt::Key_A: currDoll->setLaunchDirection(Left); break;
    case Qt::Key_D: currDoll->setLaunchDirection(Right); break;
    case Qt::Key_Space: launch(); break;
    }
    update();
}

void MainWindow::launch() {
    currDoll->arrowCount = 0;
    currDoll->stepCount = 0;
    currDoll->applyLaunchDirection();
    currDoll->smoothX = currDoll->x;
    currDoll->smoothY = currDoll->y;
    currDoll->isMoving = true;
    moveTimer->start();
    animTimer->start();
}

void MainWindow::updateMoveAnimation() {
    const int oldX = currDoll->x;
    const int oldY = currDoll->y;
    currDoll->moveStep();
    currDoll->stepCount++;
    const int x = currDoll->x;
    const int y = currDoll->y;

    if (currDoll->stepCount > Doll::MAX_STEPS) {
        QMessageBox::information(this, "步数超限",
            QString("角色移动超过 %1 步，疑似陷入循环，本轮行动取消！").arg(Doll::MAX_STEPS));
        currDoll->x = oldX;
        currDoll->y = oldY;
        currDoll->smoothX = oldX;
        currDoll->smoothY = oldY;
        currDoll->isMoving = false;
        moveTimer->stop();
        animTimer->stop();
        board->refreshArrows(doll1->x, doll1->y, doll2->x, doll2->y);
        board->spawnPortals(doll1->x, doll1->y, doll2->x, doll2->y);
        switchPlayer();
        return;
    }

    if (isOutOfBounds(x, y)) {
        currDoll->x = oldX;
        currDoll->y = oldY;
        currDoll->smoothX = oldX;
        currDoll->smoothY = oldY;
        currDoll->isMoving = false;
        moveTimer->stop();
        animTimer->stop();
        board->refreshArrows(doll1->x, doll1->y, doll2->x, doll2->y);
        board->spawnPortals(doll1->x, doll1->y, doll2->x, doll2->y);
        switchPlayer();
        return;
    }

    Cell cell = board->getCell(x, y);
    Doll* enemy = getEnemy();

    if (cell.isBlock) {
        currDoll->x = oldX;
        currDoll->y = oldY;
        currDoll->smoothX = oldX;
        currDoll->smoothY = oldY;
        currDoll->isMoving = false;
        moveTimer->stop();
        animTimer->stop();
        board->refreshArrows(doll1->x, doll1->y, doll2->x, doll2->y);
        board->spawnPortals(doll1->x, doll1->y, doll2->x, doll2->y);
        switchPlayer();
        return;
    }

    if (cell.hasPortal) {
        QPoint otherPortal = board->findOtherPortal(x, y);
        if (otherPortal.x() != -1) {
            currDoll->x = otherPortal.x();
            currDoll->y = otherPortal.y();
            currDoll->smoothX = currDoll->x;
            currDoll->smoothY = currDoll->y;
            PortalDirection targetDir = board->getPortalDirection(otherPortal.x(), otherPortal.y());
            currDoll->currentDir = board->getExitDirection(targetDir);
        }
    }

    if (cell.hasChest) {
        board->removeChest(x, y);
        bool isBig = (QRandomGenerator::global()->bounded(2) == 0);
        if (isBig) {
            currDoll->baseDamage += CHEST_BIG_BONUS;
            currDoll->bigSwordCount++;
        } else {
            currDoll->baseDamage += CHEST_SMALL_BONUS;
            currDoll->smallSwordCount++;
        }
        currDoll->chestCollected++;
    }

    if (cell.hasHealthPack) {
        board->removeHealthPack(x, y);
        currDoll->heal(GameBoard::HEALTH_PACK_AMOUNT);
        currDoll->hpCollected++;
    }

    if (x == enemy->x && y == enemy->y) {
        currDoll->x = oldX;
        currDoll->y = oldY;
        currDoll->smoothX = oldX;
        currDoll->smoothY = oldY;

        const int arrowDamage = currDoll->arrowCount * DAMAGE_PER_ARROW;
        const int totalDamage = arrowDamage + currDoll->baseDamage;
        enemy->hurt(totalDamage);

        QString damageInfo = QString("途经箭头: %1 × %2 = %3\n基础加成: %4\n总伤害: %5")
                                 .arg(currDoll->arrowCount)
                                 .arg(DAMAGE_PER_ARROW)
                                 .arg(arrowDamage)
                                 .arg(currDoll->baseDamage)
                                 .arg(totalDamage);
        QMessageBox::information(this, "命中", damageInfo);

        if (doll1->isDead()) {
            gameOver(2);
        } else if (doll2->isDead()) {
            gameOver(1);
        }

        currDoll->isMoving = false;
        moveTimer->stop();
        animTimer->stop();
        board->refreshArrows(doll1->x, doll1->y, doll2->x, doll2->y);
        board->spawnPortals(doll1->x, doll1->y, doll2->x, doll2->y);
        switchPlayer();
        return;
    }

    if (cell.hasArrow) {
        currDoll->currentDir = cell.dir;
        currDoll->arrowCount++;
    }

    update();
}

void MainWindow::updateAnimFrame() {
    currDoll->smoothX += (currDoll->x - currDoll->smoothX) * 0.4f;
    currDoll->smoothY += (currDoll->y - currDoll->smoothY) * 0.4f;
    currDoll->animFrame = (currDoll->animFrame + 1) % 3;
    update();
}

void MainWindow::updateTurnTimer() {
    if (gameEnd || moveTimer->isActive() || isPaused) return;
    if (--turnTime <= 0) {
        switchPlayer();
    }
    update();
}

void MainWindow::switchPlayer() {
    Doll* prevDoll = currDoll;
    
    if (prevDoll->chestCollected > 0) {
        const int totalBonus = prevDoll->bigSwordCount * CHEST_BIG_BONUS
                             + prevDoll->smallSwordCount * CHEST_SMALL_BONUS;
        QMessageBox::information(this, "获得宝箱",
            QString("收集了 %1 个宝箱！\n大剑 ×%2 (+%3)\n小剑 ×%4 (+%5)\n总加成: +%6\n当前基础伤害: %7")
                .arg(prevDoll->chestCollected)
                .arg(prevDoll->bigSwordCount)
                .arg(prevDoll->bigSwordCount * CHEST_BIG_BONUS)
                .arg(prevDoll->smallSwordCount)
                .arg(prevDoll->smallSwordCount * CHEST_SMALL_BONUS)
                .arg(totalBonus)
                .arg(prevDoll->baseDamage));
        prevDoll->chestCollected = 0;
        prevDoll->bigSwordCount = 0;
        prevDoll->smallSwordCount = 0;
    }

    if (prevDoll->hpCollected > 0) {
        QMessageBox::information(this, "获得血包",
            QString("收集了 %1 个血包！\n回复血量 +%2\n当前血量: %3")
                .arg(prevDoll->hpCollected)
                .arg(prevDoll->hpCollected * GameBoard::HEALTH_PACK_AMOUNT)
                .arg(prevDoll->hp));
        prevDoll->hpCollected = 0;
    }
    
    currPlayer = (currPlayer == 1) ? 2 : 1;
    turnTime = TURN_TIME;
    currDoll = (currPlayer == 1) ? doll1.get() : doll2.get();
    board->spawnArrow(doll1->x, doll1->y, doll2->x, doll2->y);

    turnCounter++;
    if (turnCounter % 2 == 0) {
        board->refreshHealthPacks(doll1->x, doll1->y, doll2->x, doll2->y);
    }

    update();
}

void MainWindow::gameOver(int winner) {
    gameEnd = true;
    QMessageBox::information(this, "游戏结束", QString("玩家%1 胜利！").arg(winner));
    QMessageBox::information(this, "重置", "确定后重新开始");

    restartGame();
}

void MainWindow::loadP1Sprites() {
    const char* dirNames[] = { "右面", "正面", "左面", "背面" };
    for (int d = 0; d < 4; ++d) {
        for (int f = 0; f < 3; ++f) {
            QString path = QString(":/p1%1%2.jpg").arg(dirNames[d]).arg(f + 1);
            p1Sprites[d][f] = QPixmap(path);
        }
    }
}

QPixmap MainWindow::getP1Sprite(Direction dir, int frame) const {
    int d = static_cast<int>(dir);
    return p1Sprites[d][frame % 3];
}

void MainWindow::loadP2Sprites() {
    const char* dirNames[] = { "右面", "正面", "左面", "背面" };
    for (int d = 0; d < 4; ++d) {
        for (int f = 0; f < 3; ++f) {
            QString path = QString(":/p2%1%2.jpg").arg(dirNames[d]).arg(f + 1);
            p2Sprites[d][f] = QPixmap(path);
        }
    }
}

QPixmap MainWindow::getP2Sprite(Direction dir, int frame) const {
    int d = static_cast<int>(dir);
    return p2Sprites[d][frame % 3];
}

void MainWindow::restartGame() {
    doll1->reset();
    doll2->reset();
    board->initBoard();
    gameEnd = false;
    isPaused = false;
    currPlayer = 1;
    turnTime = TURN_TIME;
    turnCounter = 0;
    currDoll = doll1.get();
    turnTimer->start(1000);
    pauseButton->show();
    update();
}

void MainWindow::pauseGame() {
    isPaused = true;
    bool wasMoving = moveTimer->isActive();
    turnTimer->stop();
    moveTimer->stop();
    animTimer->stop();
    pauseButton->hide();

    QDialog dialog(this);
    dialog.setWindowTitle("游戏暂停");
    dialog.setFixedSize(280, 240);
    dialog.setStyleSheet("QDialog { background-color: #1a1a2e; }");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(16);

    QLabel *titleLabel = new QLabel("游戏暂停", &dialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");

    QPushButton *btnResume = new QPushButton("继续游戏", &dialog);
    QPushButton *btnRestart = new QPushButton("重新开始", &dialog);
    QPushButton *btnExit = new QPushButton("结束游戏", &dialog);

    QString btnStyle =
        "QPushButton { background-color: #16213e; color: white; font-size: 16px; "
        "border: 2px solid #0f3460; border-radius: 8px; padding: 10px 30px; }"
        "QPushButton:hover { background-color: #0f3460; }";
    btnResume->setStyleSheet(btnStyle);
    btnRestart->setStyleSheet(btnStyle);
    btnExit->setStyleSheet(btnStyle);

    btnResume->setCursor(Qt::PointingHandCursor);
    btnRestart->setCursor(Qt::PointingHandCursor);
    btnExit->setCursor(Qt::PointingHandCursor);

    layout->addWidget(titleLabel);
    layout->addSpacing(8);
    layout->addWidget(btnResume);
    layout->addWidget(btnRestart);
    layout->addWidget(btnExit);

    int action = 0;
    connect(btnResume, &QPushButton::clicked, [&]() { action = 1; dialog.accept(); });
    connect(btnRestart, &QPushButton::clicked, [&]() { action = 2; dialog.accept(); });
    connect(btnExit, &QPushButton::clicked, [&]() { action = 3; dialog.accept(); });

    dialog.exec();

    isPaused = false;
    pauseButton->show();

    if (action == 1) {
        turnTimer->start(1000);
        if (wasMoving) { moveTimer->start(); animTimer->start(); }
    } else if (action == 2) {
        restartGame();
    } else if (action == 3) {
        emit returnToStart();
        close();
    } else {
        turnTimer->start(1000);
        if (wasMoving) { moveTimer->start(); animTimer->start(); }
    }
    update();
}
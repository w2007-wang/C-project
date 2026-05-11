#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QKeyEvent>
#include <QPushButton>
#include <QDialog>
#include <memory>
#include "gameboard.h"
#include "doll.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    static constexpr int BOARD_MARGIN = 80;
    static constexpr int TURN_TIME = 60;
    static constexpr int STEP_DURATION = 160;
    static constexpr int ANIM_INTERVAL = 16;
    static constexpr int DAMAGE_PER_ARROW = 4;
    static constexpr int CHEST_SMALL_BONUS = 12;
    static constexpr int CHEST_BIG_BONUS = 20;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void restartGame();

signals:
    void returnToStart();
    void showResult(int winner);

protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateTurnTimer();
    void updateAnimFrame();
    void switchPlayer();
    void gameOver(int winner);
    void pauseGame();

private:
    QRect getBoardRect() const;
    int getCellSize() const;
    QPoint posToGrid(const QPoint &pos) const;
    QPoint gridToPos(int x, int y) const;
    QPointF gridToPosF(float x, float y) const;

    void drawBoard(QPainter &p);
    void drawArrow(QPainter &p, const QRect &cellRect, Direction dir, int cellSize);
    void drawChest(QPainter &p, const QRect &cellRect, int cellSize);
    void drawPortal(QPainter &p, const QRect &cellRect, int cellSize, PortalDirection dir);
    void drawHealthPack(QPainter &p, const QRect &cellRect, int cellSize);
    void drawDoll(QPainter &p, const Doll *d, const QColor &color);
    void drawHP(QPainter &p);
    void launch();
    bool processStep();
    void stopMovingAndSwitch();
    void loadP1Sprites();
    QPixmap getP1Sprite(Direction dir, int frame) const;
    void loadP2Sprites();
    QPixmap getP2Sprite(Direction dir, int frame) const;

    bool isOutOfBounds(int x, int y) const;
    Doll* getEnemy() const;

    std::unique_ptr<GameBoard> board;
    std::unique_ptr<Doll> doll1;
    std::unique_ptr<Doll> doll2;
    Doll *currDoll = nullptr;

    QTimer *turnTimer = nullptr;
    QTimer *animTimer = nullptr;
    QPushButton *pauseButton = nullptr;
    int turnTime = TURN_TIME;
    int currPlayer = 1;
    int turnCounter = 0;
    int animCounter = 0;
    bool gameEnd = false;
    bool isPaused = false;
    QPixmap p1Sprites[4][3];
    QPixmap p2Sprites[4][3];
};

#endif
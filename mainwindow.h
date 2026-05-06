#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
<<<<<<< HEAD
#include <QPainterPath>
=======
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc
#include <QTimer>
#include <QKeyEvent>
#include "gameboard.h"
#include "doll.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateTurnTimer();
    void updateMoveAnimation();
    void switchPlayer();
    void gameOver(int winner);

private:
<<<<<<< HEAD
    QRect getBoardRect() const;
    int getCellSize() const;
=======
    QRect getBoardRect() const;       // 棋盘所在区域
    int getCellSize() const;          // 自动计算格子大小
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc
    QPoint posToGrid(const QPoint &pos) const;
    QPoint gridToPos(int x, int y) const;

    void drawBoard(QPainter &p);
<<<<<<< HEAD
    void drawArrow(QPainter &p, const QRect &cellRect, Direction dir, int cellSize);
    void drawChest(QPainter &p, const QRect &cellRect, int cellSize);
=======
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc
    void drawDoll(QPainter &p, Doll *d, const QColor &color);
    void drawHP(QPainter &p);
    void launch();

    GameBoard *board;
    Doll *doll1;
    Doll *doll2;
    Doll *currDoll;

    QTimer *turnTimer;
    QTimer *moveTimer;
    int turnTime;
    int currPlayer;
    bool gameEnd;

    int m_boardMargin;
};

<<<<<<< HEAD
#endif
=======
#endif
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc

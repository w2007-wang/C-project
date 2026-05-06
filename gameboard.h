#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <QObject>

enum Direction { Right, Down, Left, Up, DirCount };

struct Cell {
    bool hasArrow = false;
    Direction dir = Right;
    bool isBlock = false;
    bool hasChest = false;
};

class GameBoard : public QObject
{
    Q_OBJECT
public:
    static const int SIZE = 9;
    static const int CHEST_COUNT = 2;
    explicit GameBoard(QObject *parent = nullptr);
    void initBoard();
    void rotateArrow(int x, int y);
    Cell getCell(int x, int y);
    void spawnArrow(int player1X, int player1Y, int player2X, int player2Y);
    void refreshArrows(int player1X, int player1Y, int player2X, int player2Y);
    void spawnChest(int player1X, int player1Y, int player2X, int player2Y);
    void removeChest(int x, int y);

private:
    Cell board[SIZE][SIZE];
    const int ARROW_COUNT = 27;
};

#endif
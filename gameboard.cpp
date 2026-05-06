<<<<<<< HEAD
#include "gameboard.h"
=======
         #include "gameboard.h"
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc
#include <QRandomGenerator>

GameBoard::GameBoard(QObject *parent) : QObject(parent) {
    initBoard();
}

void GameBoard::initBoard() {
    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            board[y][x] = Cell();

    int cnt = 0;
    while (cnt < ARROW_COUNT) {
        int x = QRandomGenerator::global()->bounded(SIZE);
        int y = QRandomGenerator::global()->bounded(SIZE);
        if (!board[y][x].hasArrow && !(x==1&&y==1) && !(x==7&&y==7)) {
            board[y][x].hasArrow = true;
            board[y][x].dir = (Direction)QRandomGenerator::global()->bounded(DirCount);
            cnt++;
        }
    }

    for (int i = 0; i < 8; ++i) {
        int x = QRandomGenerator::global()->bounded(SIZE);
        int y = QRandomGenerator::global()->bounded(SIZE);
        if (!board[y][x].hasArrow)
            board[y][x].isBlock = true;
    }
<<<<<<< HEAD

    for (int i = 0; i < CHEST_COUNT; ++i) {
        spawnChest(1, 1, 7, 7);
    }
=======
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc
}

void GameBoard::rotateArrow(int x, int y) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return;
    if (board[y][x].hasArrow)
        board[y][x].dir = (Direction)((board[y][x].dir + 1) % DirCount);
}

Cell GameBoard::getCell(int x, int y) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return Cell();
    return board[y][x];
}
<<<<<<< HEAD

void GameBoard::spawnArrow(int player1X, int player1Y, int player2X, int player2Y) {
    for (int attempt = 0; attempt < 100; ++attempt) {
        int x = QRandomGenerator::global()->bounded(SIZE);
        int y = QRandomGenerator::global()->bounded(SIZE);

        bool isOccupied = (x == player1X && y == player1Y) ||
                          (x == player2X && y == player2Y) ||
                          board[y][x].hasArrow ||
                          board[y][x].isBlock;

        if (!isOccupied) {
            board[y][x].hasArrow = true;
            board[y][x].dir = (Direction)QRandomGenerator::global()->bounded(DirCount);
            break;
        }
    }
}

void GameBoard::refreshArrows(int player1X, int player1Y, int player2X, int player2Y) {
    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            board[y][x].hasArrow = false;

    int cnt = 0;
    while (cnt < ARROW_COUNT) {
        int x = QRandomGenerator::global()->bounded(SIZE);
        int y = QRandomGenerator::global()->bounded(SIZE);

        bool isOccupied = (x == player1X && y == player1Y) ||
                          (x == player2X && y == player2Y) ||
                          board[y][x].hasArrow ||
                          board[y][x].isBlock;

        if (!isOccupied) {
            board[y][x].hasArrow = true;
            board[y][x].dir = (Direction)QRandomGenerator::global()->bounded(DirCount);
            cnt++;
        }
    }
}

void GameBoard::spawnChest(int player1X, int player1Y, int player2X, int player2Y) {
    for (int attempt = 0; attempt < 100; ++attempt) {
        int x = QRandomGenerator::global()->bounded(SIZE);
        int y = QRandomGenerator::global()->bounded(SIZE);

        bool isOccupied = (x == player1X && y == player1Y) ||
                          (x == player2X && y == player2Y) ||
                          board[y][x].hasArrow ||
                          board[y][x].isBlock ||
                          board[y][x].hasChest;

        if (!isOccupied) {
            board[y][x].hasChest = true;
            break;
        }
    }
}

void GameBoard::removeChest(int x, int y) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return;
    board[y][x].hasChest = false;
}
=======
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc

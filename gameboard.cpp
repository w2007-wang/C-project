         #include "gameboard.h"
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

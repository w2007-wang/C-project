#include "gameboard.h"
#include <random>
#include <QPoint>

void GameBoard::initBoard() {
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            board[y][x] = Cell{};
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, SIZE - 1);

    for (int i = 0; i < ARROW_COUNT; ++i) {
        int x, y;
        do {
            x = dis(gen);
            y = dis(gen);
        } while (board[y][x].hasArrow || (x == 1 && y == 1) || (x == 7 && y == 7));
        board[y][x].hasArrow = true;
        board[y][x].dir = randomDirection();
    }

    for (int i = 0; i < BLOCK_COUNT; ++i) {
        int x, y;
        do {
            x = dis(gen);
            y = dis(gen);
        } while (board[y][x].hasArrow || (x == 1 && y == 1) || (x == 7 && y == 7));
        board[y][x].isBlock = true;
    }

    for (int i = 0; i < CHEST_COUNT; ++i) {
        spawnChest(1, 1, 7, 7);
    }

    spawnPortals(1, 1, 7, 7);
    spawnHealthPacks(1, 1, 7, 7);
}

void GameBoard::rotateArrow(int x, int y) {
    if (!isValidPosition(x, y)) return;
    if (board[y][x].hasArrow) {
        board[y][x].dir = static_cast<Direction>((board[y][x].dir + 1) % DirCount);
    }
}

Cell GameBoard::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) return Cell{};
    return board[y][x];
}

void GameBoard::spawnArrow(int player1X, int player1Y, int player2X, int player2Y) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, SIZE - 1);

    for (int attempt = 0; attempt < 100; ++attempt) {
        int x = dis(gen);
        int y = dis(gen);
        if (!isPositionOccupied(x, y, player1X, player1Y, player2X, player2Y)) {
            board[y][x].hasArrow = true;
            board[y][x].dir = randomDirection();
            break;
        }
    }
}

void GameBoard::refreshArrows(int player1X, int player1Y, int player2X, int player2Y) {
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            board[y][x].hasArrow = false;
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, SIZE - 1);

    for (int i = 0; i < ARROW_COUNT; ++i) {
        int x, y;
        do {
            x = dis(gen);
            y = dis(gen);
        } while (isPositionOccupied(x, y, player1X, player1Y, player2X, player2Y));
        board[y][x].hasArrow = true;
        board[y][x].dir = randomDirection();
    }
}

void GameBoard::spawnChest(int player1X, int player1Y, int player2X, int player2Y) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, SIZE - 1);

    for (int attempt = 0; attempt < 100; ++attempt) {
        int x = dis(gen);
        int y = dis(gen);
        if (!isPositionOccupied(x, y, player1X, player1Y, player2X, player2Y, true)) {
            board[y][x].hasChest = true;
            break;
        }
    }
}

void GameBoard::removeChest(int x, int y) {
    if (!isValidPosition(x, y)) return;
    board[y][x].hasChest = false;
}

bool GameBoard::isValidPosition(int x, int y) const {
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

bool GameBoard::isPositionOccupied(int x, int y, int player1X, int player1Y, 
                                    int player2X, int player2Y, bool checkChest, bool checkPortal, bool checkHealthPack) const {
    return (x == player1X && y == player1Y) ||
           (x == player2X && y == player2Y) ||
           board[y][x].hasArrow ||
           board[y][x].isBlock ||
           (checkChest && board[y][x].hasChest) ||
           (checkPortal && board[y][x].hasPortal) ||
           (checkHealthPack && board[y][x].hasHealthPack);
}

Direction GameBoard::randomDirection() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, DirCount - 1);
    return static_cast<Direction>(dis(gen));
}

void GameBoard::rotatePortal(int x, int y) {
    if (!isValidPosition(x, y) || !board[y][x].hasPortal) return;
    board[y][x].portalDir = static_cast<PortalDirection>((board[y][x].portalDir + 1) % PortalDirCount);
}

bool GameBoard::hasPortal(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    return board[y][x].hasPortal;
}

PortalDirection GameBoard::getPortalDirection(int x, int y) const {
    if (!isValidPosition(x, y) || !board[y][x].hasPortal) return PortalFront;
    return board[y][x].portalDir;
}

Direction GameBoard::getExitDirection(PortalDirection dir) const {
    switch (dir) {
        case PortalFront: return Down;
        case PortalLeft:   return Left;
        case PortalRight:  return Right;
        case PortalBack:   return Up;
        default: return Down;
    }
}

QPoint GameBoard::findOtherPortal(int x, int y) const {
    if (!isValidPosition(x, y) || !board[y][x].hasPortal) {
        return QPoint(-1, -1);
    }

    for (int py = 0; py < SIZE; ++py) {
        for (int px = 0; px < SIZE; ++px) {
            if (board[py][px].hasPortal && (px != x || py != y)) {
                return QPoint(px, py);
            }
        }
    }
    return QPoint(-1, -1);
}

void GameBoard::spawnPortals(int player1X, int player1Y, int player2X, int player2Y) {
    clearPortals();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, SIZE - 1);

    for (int i = 0; i < PORTAL_COUNT; ++i) {
        for (int attempt = 0; attempt < 100; ++attempt) {
            int x = dis(gen);
            int y = dis(gen);
            if (!isPositionOccupied(x, y, player1X, player1Y, player2X, player2Y, true, true)) {
                board[y][x].hasPortal = true;
                board[y][x].portalDir = PortalFront;
                break;
            }
        }
    }
}

void GameBoard::clearPortals() {
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            board[y][x].hasPortal = false;
        }
    }
}

void GameBoard::spawnHealthPacks(int player1X, int player1Y, int player2X, int player2Y) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, SIZE - 1);

    for (int i = 0; i < HEALTH_PACK_COUNT; ++i) {
        for (int attempt = 0; attempt < 100; ++attempt) {
            int x = dis(gen);
            int y = dis(gen);
            if (!isPositionOccupied(x, y, player1X, player1Y, player2X, player2Y, true, true, true)) {
                board[y][x].hasHealthPack = true;
                break;
            }
        }
    }
}

void GameBoard::removeHealthPack(int x, int y) {
    if (!isValidPosition(x, y)) return;
    board[y][x].hasHealthPack = false;
}

void GameBoard::refreshHealthPacks(int player1X, int player1Y, int player2X, int player2Y) {
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            board[y][x].hasHealthPack = false;
        }
    }
    spawnHealthPacks(player1X, player1Y, player2X, player2Y);
}
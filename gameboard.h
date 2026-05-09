#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QPoint>

enum Direction { Right, Down, Left, Up, DirCount };

enum PortalDirection { PortalFront, PortalLeft, PortalBack, PortalRight, PortalDirCount };

struct Cell {
    bool hasArrow = false;
    Direction dir = Right;
    bool isBlock = false;
    bool hasChest = false;
    bool hasPortal = false;
    PortalDirection portalDir = PortalFront;
    bool hasHealthPack = false;
};

class GameBoard {
public:
    static constexpr int SIZE = 9;
    static constexpr int ARROW_COUNT = 27;
    static constexpr int BLOCK_COUNT = 8;
    static constexpr int CHEST_COUNT = 2;
    static constexpr int PORTAL_COUNT = 2;
    static constexpr int HEALTH_PACK_COUNT = 2;
    static constexpr int HEALTH_PACK_AMOUNT = 20;

    GameBoard() { initBoard(); }
    ~GameBoard() = default;

    void initBoard();
    void rotateArrow(int x, int y);
    void rotatePortal(int x, int y);
    Cell getCell(int x, int y) const;
    void spawnArrow(int player1X, int player1Y, int player2X, int player2Y);
    void refreshArrows(int player1X, int player1Y, int player2X, int player2Y);
    void spawnChest(int player1X, int player1Y, int player2X, int player2Y);
    void removeChest(int x, int y);
    bool hasPortal(int x, int y) const;
    PortalDirection getPortalDirection(int x, int y) const;
    Direction getExitDirection(PortalDirection dir) const;
    QPoint findOtherPortal(int x, int y) const;
    void spawnPortals(int player1X, int player1Y, int player2X, int player2Y);
    void clearPortals();
    void spawnHealthPacks(int player1X, int player1Y, int player2X, int player2Y);
    void removeHealthPack(int x, int y);
    void refreshHealthPacks(int player1X, int player1Y, int player2X, int player2Y);

private:
    Cell board[SIZE][SIZE]{};

    bool isValidPosition(int x, int y) const;
    bool isPositionOccupied(int x, int y, int player1X, int player1Y, 
                            int player2X, int player2Y, bool checkChest = false, bool checkPortal = false, bool checkHealthPack = false) const;
    Direction randomDirection() const;
};

#endif
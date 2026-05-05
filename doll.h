#ifndef DOLL_H
#define DOLL_H
#include <QObject>
#include "gameboard.h"

class Doll : public QObject
{
    Q_OBJECT
public:
    int x, y;
    int player;
    int hp;
    Direction currentDir;   // 移动中方向
    Direction launchDir;    // 出发初始方向
    bool isMoving;
    int arrowCount;

    explicit Doll(int player, QObject *parent = nullptr);
    void reset();
    void setStartPos(int x, int y);
    void moveStep();
    void setLaunchDirection(Direction dir); // 关键：设置出发方向
    void applyLaunchDirection();             // 出发时应用
    bool checkCollide(Doll *enemy);
    void hurt(int damage);
    bool isDead() const;
};

#endif

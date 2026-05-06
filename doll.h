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
<<<<<<< HEAD
    int baseDamage;
    Direction currentDir;
    Direction launchDir;
=======
    Direction currentDir;   // 移动中方向
    Direction launchDir;    // 出发初始方向
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc
    bool isMoving;
    int arrowCount;

    explicit Doll(int player, QObject *parent = nullptr);
    void reset();
    void setStartPos(int x, int y);
    void moveStep();
<<<<<<< HEAD
    void setLaunchDirection(Direction dir);
    void applyLaunchDirection();
=======
    void setLaunchDirection(Direction dir); // 关键：设置出发方向
    void applyLaunchDirection();             // 出发时应用
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc
    bool checkCollide(Doll *enemy);
    void hurt(int damage);
    bool isDead() const;
};

<<<<<<< HEAD
#endif
=======
#endif
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc

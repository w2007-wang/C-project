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
    int baseDamage;
    Direction currentDir;
    Direction launchDir;
    bool isMoving;
    int arrowCount;

    explicit Doll(int player, QObject *parent = nullptr);
    void reset();
    void setStartPos(int x, int y);
    void moveStep();
    void setLaunchDirection(Direction dir);
    void applyLaunchDirection();
    bool checkCollide(Doll *enemy);
    void hurt(int damage);
    bool isDead() const;
};

#endif
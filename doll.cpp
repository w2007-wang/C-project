#include "doll.h"

Doll::Doll(int player, QObject *parent) : QObject(parent)
{
    this->player = player;
    reset();
}

void Doll::reset()
{
    hp = 100;
<<<<<<< HEAD
    baseDamage = 0;
=======
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc
    isMoving = false;
    currentDir = Right;
    launchDir = Right;
    arrowCount = 0;

    if (player == 1)
        setStartPos(1, 1);
    else
        setStartPos(7, 7);
}

void Doll::setStartPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Doll::setLaunchDirection(Direction dir)
{
    if (!isMoving) {
        launchDir = dir;
    }
}

void Doll::applyLaunchDirection()
{
    currentDir = launchDir;
}

void Doll::moveStep()
{
    switch (currentDir) {
    case Right: x++; break;
    case Down:  y++; break;
    case Left:  x--; break;
    case Up:    y--; break;
    default: break;
    }
}

bool Doll::checkCollide(Doll *enemy)
{
    return x == enemy->x && y == enemy->y;
}

void Doll::hurt(int damage)
{
    hp -= damage;
    if (hp < 0) hp = 0;
}

bool Doll::isDead() const
{
    return hp <= 0;
<<<<<<< HEAD
}
=======
}
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc

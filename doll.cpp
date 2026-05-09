#include "doll.h"

Doll::Doll(int player) : player(player) {
    reset();
}

void Doll::reset() {
    hp = INITIAL_HP;
    baseDamage = 0;
    isMoving = false;
    currentDir = Down;
    launchDir = Down;
    arrowCount = 0;
    stepCount = 0;
    chestCollected = 0;
    bigSwordCount = 0;
    smallSwordCount = 0;
    hpCollected = 0;
    animFrame = 0;

    if (player == 1) {
        setStartPos(PLAYER1_START_X, PLAYER1_START_Y);
    } else {
        setStartPos(PLAYER2_START_X, PLAYER2_START_Y);
    }
}

void Doll::setStartPos(int x, int y) {
    this->x = x;
    this->y = y;
    smoothX = x;
    smoothY = y;
}

void Doll::setLaunchDirection(Direction dir) {
    if (!isMoving) {
        launchDir = dir;
        currentDir = dir;
        animFrame = 0;
    }
}

void Doll::applyLaunchDirection() {
    currentDir = launchDir;
}

void Doll::moveStep() {
    switch (currentDir) {
    case Right: x++; break;
    case Down:  y++; break;
    case Left:  x--; break;
    case Up:    y--; break;
    default: break;
    }
}

bool Doll::checkCollide(const Doll *enemy) const {
    return x == enemy->x && y == enemy->y;
}

void Doll::hurt(int damage) {
    hp -= damage;
    if (hp < 0) {
        hp = 0;
    }
}

void Doll::heal(int amount) {
    hp += amount;
    if (hp > INITIAL_HP) {
        hp = INITIAL_HP;
    }
}

bool Doll::isDead() const {
    return hp <= 0;
}
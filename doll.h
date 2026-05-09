#ifndef DOLL_H
#define DOLL_H
#include "gameboard.h"

class Doll {
public:
    static constexpr int INITIAL_HP = 100;
    static constexpr int PLAYER1_START_X = 1;
    static constexpr int PLAYER1_START_Y = 1;
    static constexpr int PLAYER2_START_X = 7;
    static constexpr int PLAYER2_START_Y = 7;
    static constexpr int MAX_STEPS = 200;

    int x = PLAYER1_START_X;
    int y = PLAYER1_START_Y;
    int player = 1;
    int hp = INITIAL_HP;
    int baseDamage = 0;
    Direction currentDir = Down;
    Direction launchDir = Down;
    bool isMoving = false;
    int arrowCount = 0;
    int stepCount = 0;
    int chestCollected = 0;
    int bigSwordCount = 0;
    int smallSwordCount = 0;
    int hpCollected = 0;
    int animFrame = 0;
    float smoothX = PLAYER1_START_X;
    float smoothY = PLAYER1_START_Y;

    explicit Doll(int player);
    ~Doll() = default;

    void reset();
    void setStartPos(int x, int y);
    void moveStep();
    void setLaunchDirection(Direction dir);
    void applyLaunchDirection();
    bool checkCollide(const Doll *enemy) const;
    void hurt(int damage);
    void heal(int amount);
    bool isDead() const;
};

#endif
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor: public GraphObject
{
public:
    Actor(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~Actor();
    virtual void doSomething()=0;
    StudentWorld*getWorld();
    bool getAlive();
    void setAliveStatus(bool s);
    virtual bool isSquare();
    virtual bool isDirectionalSquare();
    bool isWaiting();
    void setIsWaiting(bool set);
    int getMoveDirection();
    void setMoveDirection(int a);
    void setNewStatus(bool s);
    bool getNewStatus();
    void minusTicks();
    void setTicks(int m);
    int getTicks();
    void keepMoving();
    void goRandomDirection();
    virtual bool canBeVortexed();
    void teleport();
    
private:
    StudentWorld* m_world;
    bool isAlive;
    bool waitingToMove;
    int moveDirection;
    bool isNew;
    int ticks_to_move;
};

class Player: public Actor
{
public:
    Player(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size, int playerNum);
    virtual ~Player();
    virtual void doSomething();
    void addCoins(int c);
    void resetCoins();
    int getCoins();
    void addStars(int s);
    void setCoins(int c);
    void setStars(int s);
    int getStars();
    int dieRoll();
    void forceDirection(int d);
    void giveVortex(bool b);
    bool returnHasVortex();
private:
    int m_playerNum;
    int die_roll;
    int coins;
    int stars;
    bool hasVortex;
};

class Yoshi: public Player
{
public:
    Yoshi(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~Yoshi();
};

class Peach: public Player
{
public:
    Peach(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~Peach();
};

class Square: public Actor
{
public:
    Square(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~Square();
    virtual void doSomething();
    virtual bool isSquare();
};

class CoinSquare: public Square
{
public:
    CoinSquare(bool color, StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~CoinSquare();
    bool getColor();
    virtual void doSomething();
    
private:
    bool isBlue;
    
};

//FOR PART 2

class StarSquare: public Square
{
public:
    StarSquare(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~StarSquare();
    virtual void doSomething();
private:
    
};

class DirectionalSquare: public Square
{
public:
    DirectionalSquare(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~DirectionalSquare();
    virtual void doSomething();
    virtual bool isDirectionalSquare();
    
};

class BankSquare: public Square
{
public:
    BankSquare(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~BankSquare();
    virtual void doSomething();
    
};

class EventSquare: public Square
{
public:
    EventSquare(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~EventSquare();
    virtual void doSomething();
    
};

class DroppingSquare: public Square
{
public:
    DroppingSquare(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~DroppingSquare();
    virtual void doSomething();
    
};

class Baddie: public Actor
{
public:
    Baddie(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~Baddie();
    virtual void doSomething();
    void hit_by_vortex();
    void decrementBaddieCounter();
    int getBaddieCounter();
    void setBaddieCounter(int s);
    virtual bool canBeVortexed();
private:
    int baddie_counter;
    
};

class Bowser: public Baddie
{
public:
    Bowser(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~Bowser();
    virtual void doSomething();
    
};

class Boo: public Baddie
{
public:
    Boo(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    virtual ~Boo();
    virtual void doSomething();
    
};

class Vortex: public Actor
{
public:
    Vortex(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size);
    ~Vortex();
    virtual void doSomething();
private:
    int direction;
    
};
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_

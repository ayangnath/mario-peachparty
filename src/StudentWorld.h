#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <vector>
#include <string>

class Actor;
class Player;
class Yoshi;
class Peach;
class Baddie;
class Vortex;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    //bool canMove(Player*p);
    bool canMove(Actor*p, int direction);
    bool overlap(Actor*a, Actor*b);
    Player* getPeach();
    Player* getYoshi();
    Player* getPlayer(int i);
    void addToBank(int b);
    void resetBank();
    int getBank();
    Actor* getRandomActor();
    bool isOnDirectional(Player*p);
    bool isAtFork(Player*p);
    bool isAtFork(Baddie*b);
    void insertDroppingSquare(Baddie*b);
    ~StudentWorld();
    bool vortexTouches(Vortex* v);
    void shootVortex(Player*p);
    
private:
    std::vector<Actor*> actors;
    Yoshi* m_yoshi;
    Peach* m_peach;
    int m_bank;
    Board bd;
};

#endif // STUDENTWORLD_H_

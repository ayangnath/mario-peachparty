#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_yoshi = nullptr;
    m_peach = nullptr;
    m_bank = 0;
}

//inserts a Dropping Square and removes the square at the place inserted
void StudentWorld::insertDroppingSquare(Baddie* b)
{
    int x = b->getX();
    int y = b->getY();
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it!= actors.end(); it++)
    {
        if ((*it)->isSquare() && (*it)->getX() == x && (*it)->getY() == y)
        {
            DroppingSquare*b = new DroppingSquare(this, IID_DROPPING_SQUARE, (*it)->getX(), (*it)->getY(), 0, 1, 1);
            actors.push_back(b);
            delete *it;
            //(*it)->setAliveStatus(false);
            it = actors.erase(it);
        }
    }
}

//shoots vortex and displays it on the screen and sets it in the correct direction to move
void StudentWorld::shootVortex(Player *p)
{
    if (p->getMoveDirection()==GraphObject::up) //case for up
    {
        Vortex*v = new Vortex(this, IID_VORTEX, p->getX(), p->getY() + 16, p->getMoveDirection(), 0, 1);
        actors.push_back(v);
    }
    else if (p->getMoveDirection()==GraphObject::down) //case for down
    {
        Vortex*v = new Vortex(this, IID_VORTEX, p->getX(), p->getY() - 16, p->getMoveDirection(), 0, 1);
        actors.push_back(v);
        
    }
    else if (p->getMoveDirection()==GraphObject::right) //case for right
    {
        Vortex*v = new Vortex(this, IID_VORTEX, p->getX() + 16, p->getY(), p->getMoveDirection(), 0, 1);
        actors.push_back(v);
        
    }
    else //case for left
    {
        Vortex*v = new Vortex(this, IID_VORTEX, p->getX()-16, p->getY(), p->getMoveDirection(), 0, 1);
        actors.push_back(v);
        
    }
}

//returns true of a Vortex touches something, and teleports the Baddie that it touches
bool StudentWorld::vortexTouches(Vortex *v)
{
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it!= actors.end(); it++)
    {
        if ((*it)->canBeVortexed())
        {
            int differenceX = v->getX() - (*it)->getX();
            int differenceY = v->getY() - (*it)->getY();
            if (abs(differenceX) <=15 && abs(differenceY) <=15 )
            {
                //teleporting the baddie
                (*it)->setIsWaiting(true);
                Actor* temp = getRandomActor();
                int randX = temp->getX();
                int randY = temp->getY();
                (*it)->moveTo(randX, randY);
                (*it)->setNewStatus(false);
                playSound(SOUND_HIT_BY_VORTEX);
                v->setAliveStatus(false);
                (*it)->goRandomDirection();
                return true;
            }
        }
    }
    return false;
    
}

//returns true if at fork, false otherwise
bool StudentWorld::isAtFork(Player*p)
{
    if (!isOnDirectional(p) && p->getX()%16==0 && p->getY()%16==0)
    {
        int count = 0;
        if (canMove(p, GraphObject::up))
            count = count +1;
        if (canMove(p, GraphObject::down))
            count = count +1;
        if (canMove(p, GraphObject::right))
            count = count +1;
        if (canMove(p, GraphObject::left))
            count = count +1;
        return count >= 3;
    }
    else return false;
}

//returns true if Baddie is at a fork
bool StudentWorld::isAtFork(Baddie*b)
{
    if (b->getX()%16==0 && b->getY()%16==0)
    {
        int count = 0;
        if (canMove(b, GraphObject::up))
            count = count +1;
        if (canMove(b, GraphObject::down))
            count = count +1;
        if (canMove(b, GraphObject::right))
            count = count +1;
        if (canMove(b, GraphObject::left))
            count = count +1;
        return count >= 3;
    }
    else return false;
}

//return if the the player is on a DirectionalSquare
bool StudentWorld::isOnDirectional(Player*p)
{
    for (int i = 0; i <actors.size(); i++)
    {
        if (actors.at(i)->isSquare() && actors.at(i)->isDirectionalSquare())
            if (overlap(p, actors.at(i)))
                return true;
    }
    return false;
}

//by looping through the actors vector, returns a RandomActor that is a square
Actor* StudentWorld::getRandomActor()
{
    while (true)
    {
        int randIndex = randInt(0, actors.size()-1.0);
        if (actors.at(randIndex)->isSquare())
            return actors.at(randIndex);
    }

}

void StudentWorld::addToBank(int b)
{
    m_bank= m_bank + b;
}
void StudentWorld::resetBank()
{
    m_bank = 0;
}
int StudentWorld::getBank()
{
    return m_bank;
}

//returns true if can move in the direction passed through the parameter
bool StudentWorld::canMove(Actor *p, int direction)
{
    int x = p->getX();
    int y = p->getY();
    bool result = false;
    
    if (x%SPRITE_WIDTH != 0 || y%SPRITE_HEIGHT!=0)
    {
        result = true;
    }

    if (direction == GraphObject::right)
    {
        if (bd.getContentsOf(x/SPRITE_WIDTH+1, y/SPRITE_HEIGHT)!=Board::empty)
            result = true;
    }
    
    if (direction == GraphObject::left)
    {
        if (bd.getContentsOf(x/SPRITE_WIDTH-1, y/SPRITE_HEIGHT)!=Board::empty)
            result = true;
    }
    
    if (direction == GraphObject::up)
    {
        if (bd.getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT+1)!=Board::empty)
            result = true;
    }
    
    if (direction == GraphObject::down)
    {
        if (bd.getContentsOf(x/SPRITE_WIDTH, y/SPRITE_HEIGHT-1)!=Board::empty)
            result = true;
    }
    
    return result;
}

Player* StudentWorld::getPeach()
{
    return m_peach;
}
Player* StudentWorld::getYoshi()
{
    return m_yoshi;
}

//returns Player or Yoshi based on the parameter
Player* StudentWorld::getPlayer(int i)
{
    if (i == 1)
        return m_peach;
    else if (i ==2)
        return m_yoshi;
    
    else return nullptr;
}

//returns true if two Actors overlap, false otherwise
bool StudentWorld::overlap(Actor*a, Actor*b)
{
    if (a->getX()==b->getX() && a->getY()==b->getY())
        return true;
    else return false;
}

int StudentWorld::init()
{
    int input = getBoardNumber();
    string board_file = assetPath() + "board0" + to_string(input) + ".txt";
    bd.loadBoard(board_file);
    for (int i = 0; i < 16; i++)
        for (int j = 0; j<16; j++)
        {
            Board::GridEntry ge = bd.getContentsOf(i, j); // x=5, y=10
             switch (ge)
             {
                 case Board::empty:
                     break;
                 case Board::boo:
                 {
                     Boo*b = new Boo(this, IID_BOO, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 0, 1);
                     actors.push_back(b);
                     CoinSquare* c = new CoinSquare(true, this, IID_BLUE_COIN_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 1, 1);
                     actors.push_back(c);
                     break;
                 }
                 case Board::bowser:
                 {
                     Bowser*b = new Bowser(this, IID_BOWSER, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 0, 1);
                     actors.push_back(b);
                     CoinSquare* c = new CoinSquare(true, this, IID_BLUE_COIN_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 1, 1);
                     actors.push_back(c);
                     break;
                 }
                 case Board::event_square:
                 {
                     EventSquare*b = new EventSquare(this, IID_EVENT_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 1, 1);
                     actors.push_back(b);
                     break;
                 }
                 
                 case Board::bank_square:
                 {
                     BankSquare* b = new BankSquare(this, IID_BANK_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 1, 1);
                     actors.push_back(b);
                     break;
                 }
                 case Board::blue_coin_square:
                 {
                     CoinSquare* b = new CoinSquare(true, this, IID_BLUE_COIN_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 1, 1);
                     actors.push_back(b);
                     break;
                 }
                 case Board::red_coin_square:
                 {
                     CoinSquare* b = new CoinSquare(false, this, IID_RED_COIN_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 1, 1);
                     actors.push_back(b);
                     break;
                 }
                 case Board::star_square:
                 {
                     StarSquare*b = new StarSquare(this, IID_STAR_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 1, 1);
                     actors.push_back(b);
                     break;
                 }
                 case Board::up_dir_square:
                 {
                     DirectionalSquare*b = new DirectionalSquare(this, IID_DIR_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 90, 1, 1);
                     actors.push_back(b);
                     break;
                 }
                 case Board::left_dir_square:
                 {
                     DirectionalSquare*b = new DirectionalSquare(this, IID_DIR_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 180, 1, 1);
                     actors.push_back(b);
                     break;
                 }
                 case Board::right_dir_square:
                 {
                     DirectionalSquare*b = new DirectionalSquare(this, IID_DIR_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 90, 1, 1);
                     actors.push_back(b);
                     break;
                 }
                 case Board::down_dir_square:
                 {
                     DirectionalSquare*b = new DirectionalSquare(this, IID_DIR_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 270, 1, 1);
                     actors.push_back(b);
                     break;
                 }
                
                 case Board::player:
                 {
                     m_yoshi = new Yoshi(this, IID_YOSHI, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 0, 1);
                     m_peach = new Peach(this, IID_PEACH, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 0, 1);
                     CoinSquare* b = new CoinSquare(true, this, IID_BLUE_COIN_SQUARE, i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 0, 1, 1);
                     actors.push_back(b);
                     break;
                 }
             }
        }
    
    startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    m_peach->doSomething();
    m_yoshi->doSomething();
    
    for (auto it = actors.begin(); it!= actors.end();)
    {
        Actor* actor = *it;
        if(!actor->getAlive()) //removes inactive actors after each tick
        {
            it = actors.erase(it);
            delete actor;
        }
        else
        {
            actor->doSomething();
            it++;
        }
    }
    

    ostringstream textToOutput;
    if (m_peach->returnHasVortex() && m_yoshi->returnHasVortex())
    {
        textToOutput << "P1 Roll: " << m_peach->dieRoll() << " Stars: "<< m_peach->getStars() << " $$: " << m_peach->getCoins() << " VOR" <<
        " | Time: "<< timeRemaining()<< " | Bank: "<< m_bank << " | P2 Roll: " << m_yoshi->dieRoll()<< " Stars: " << m_yoshi->getStars()  << " $$: " << m_yoshi->getCoins() << " VOR" ;
    }
    else if (m_peach->returnHasVortex())
    {
        textToOutput << "P1 Roll: " << m_peach->dieRoll() << " Stars: "<< m_peach->getStars() << " $$: " << m_peach->getCoins() << " VOR" <<
        " | Time: "<< timeRemaining()<< " | Bank: "<< m_bank << " | P2 Roll: " << m_yoshi->dieRoll()<< " Stars: " << m_yoshi->getStars()  << " $$: " << m_yoshi->getCoins();
    }
    else if (m_yoshi->returnHasVortex())
    {
        textToOutput << "P1 Roll: " << m_peach->dieRoll() << " Stars: "<< m_peach->getStars() << " $$: " << m_peach->getCoins() <<
        " | Time: "<< timeRemaining()<< " | Bank: "<< m_bank << " | P2 Roll: " << m_yoshi->dieRoll()<< " Stars: " << m_yoshi->getStars()  << " $$: " << m_yoshi->getCoins() << " VOR" ;
    }
    else
    {
        textToOutput << "P1 Roll: " << m_peach->dieRoll() << " Stars: "<< m_peach->getStars() << " $$: " << m_peach->getCoins() <<
        " | Time: "<< timeRemaining()<< " | Bank: "<< m_bank << " | P2 Roll: " << m_yoshi->dieRoll()<< " Stars: " << m_yoshi->getStars()  << " $$: " << m_yoshi->getCoins() ;
    }
        
        setGameStatText(textToOutput.str());
    
    if (timeRemaining() <= 0)
    {
        if (m_peach->getStars()>m_yoshi->getStars())
        {
            setFinalScore(m_peach->getStars(), m_peach->getCoins());
            playSound(SOUND_GAME_FINISHED);
            return GWSTATUS_PEACH_WON;
        }
        else if (m_peach->getStars()<m_yoshi->getStars())
        {
            setFinalScore(m_yoshi->getStars(), m_yoshi->getCoins());
            playSound(SOUND_GAME_FINISHED);
            return GWSTATUS_YOSHI_WON;
        }
        
        else
        {
            if (m_peach->getCoins()>m_yoshi->getCoins())
            {
                setFinalScore(m_peach->getStars(), m_peach->getCoins());
                playSound(SOUND_GAME_FINISHED);
                return GWSTATUS_PEACH_WON;
            }
            else if (m_peach->getCoins()<m_yoshi->getCoins())
            {
                setFinalScore(m_yoshi->getStars(), m_yoshi->getCoins());
                playSound(SOUND_GAME_FINISHED);
                return GWSTATUS_YOSHI_WON;
            }
            
            else
            {
                int random = randInt(1, 2);
                if (random == 1)
                {
                    setFinalScore(m_yoshi->getStars(), m_yoshi->getCoins());
                    playSound(SOUND_GAME_FINISHED);
                    return GWSTATUS_YOSHI_WON;
                }
                if (random == 2)
                {
                    setFinalScore(m_peach->getStars(), m_peach->getCoins());
                    playSound(SOUND_GAME_FINISHED);
                    return GWSTATUS_PEACH_WON;
                }
            }
        }
    }
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_peach;
    m_peach = nullptr;
    
    delete m_yoshi;
    m_yoshi= nullptr;
    
    vector<Actor*>::iterator it;
    for (it = actors.begin(); it != actors.end(); it++)
    {
        if(*it != nullptr)
            delete *it;
    }
    actors.clear();
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

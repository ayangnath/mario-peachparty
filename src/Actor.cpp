#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR METHODS//////////////////////////////ACTOR METHODS////////////////////////////
//ACTOR METHODS//////////////////////////////ACTOR METHODS////////////////////////////
Actor::Actor(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): GraphObject(imageID, startX, startY, dir, depth, size)
{
    m_world = m;
    isAlive = true;
    waitingToMove = true;
    moveDirection = 0;
    isNew=false;
}

Actor::~Actor(){}

StudentWorld* Actor::getWorld()
{
    return m_world;
}

//This Actor functions makes the Actor go into a randomDirection
//this function is commonly used by Baddies and Players when they are teleported
//randomly generates an int from 1 to 4 (representing the 4 directions), and if it can go in that direction sets that to move direction
void Actor::goRandomDirection()
{
    int randDir = randInt(1, 4);
    if (randDir == 1 && getWorld()->canMove(this, up))
        setMoveDirection(up);
    else if (randDir == 2 && getWorld()->canMove(this, right))
        setMoveDirection(right);
    else if (randDir == 3 && getWorld()->canMove(this, down))
        setMoveDirection(down);
    else if (randDir == 4 && getWorld()->canMove(this, left))
        setMoveDirection(left);
    
}

//Used by Baddies and Players to keep moving around the board when they are not moving
//by calling the canMove function in StudnetWorld to decide what happens at corners/edges
void Actor::keepMoving()
{
if (!getWorld()->canMove(this, getMoveDirection()))
{
    if (getMoveDirection()==right || getMoveDirection()==left)
    {
        if (getWorld()->canMove(this, up))
        {
            setMoveDirection(up);
        }
        else if (getWorld()->canMove(this, down))
        {
            setMoveDirection(down);
        }
    }
    
    else if (getMoveDirection()==up || getMoveDirection()==down)
    {
        if (getWorld()->canMove(this, right))
        {
            setMoveDirection(right);
            //setDirection(right);
        }
        
        else if (getWorld()->canMove(this, left))
        {
            setMoveDirection(left);
            //setDirection(left);
        }
    }
}

if (getMoveDirection()==right)
{
    moveTo(getX()+2, getY());
}
else if (getMoveDirection()==left)
{
    moveTo(getX()-2, getY());
}
else if (getMoveDirection()==up)
{
    moveTo(getX(), getY()+2);
}
else if (getMoveDirection()==down)
{
    moveTo(getX(), getY()-2);
}
}

//returns of actor can be hit by a vortex
bool Actor::canBeVortexed()
{
    return false;
}

bool Actor::isDirectionalSquare()
{
    return false;
}

//sets the new status
void Actor::setNewStatus(bool s)
{
    isNew = s;
}

bool Actor::getNewStatus()
{
    return isNew;
}

//gets Alive status
bool Actor::getAlive()
{
    return isAlive;
}

//sets the Alive status; if false, will delete the Actor in  StudentWorld
void Actor::setAliveStatus(bool s)
{
    isAlive = s;
}

bool Actor::isSquare()
{
    return false;
}

bool Actor::isWaiting()
{
    return waitingToMove;
}

void Actor::setIsWaiting(bool set)
{
    waitingToMove = set;
}

int Actor::getMoveDirection()
{
    return moveDirection;
}

//sets the direction that the Actor moves based on the given parameter.
//while doing this, also sets the Sprite direction correctly
void Actor::setMoveDirection(int a)
{
    moveDirection = a;
    if (a == right)
        setDirection(right);
    if (a==up)
        setDirection(right);
    if (a==left)
        setDirection(left);
    if (a==down)
        setDirection(right);
}

void Actor::minusTicks()
{
ticks_to_move--;
}

int Actor::getTicks()
{
return ticks_to_move;
}

void Actor::setTicks(int m)
{
ticks_to_move =  m;
}

//Player METHODS//////////////////////////////player METHODS////////////////////////////
//Player METHODS//////////////////////////////player METHODS////////////////////////////
Player::Player(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size, int playerNum): Actor(m, imageID, startX, startY, dir, depth, size)
{
    m_playerNum = playerNum;
    coins = 0;
    stars = 0;
    hasVortex = false;
}

Player::~Player(){}

//gives Player a Vortex by setting the status to true; if setting to false, removes Vortex from Player
void Player::giveVortex(bool b)
{
    hasVortex = b;
}
bool Player::returnHasVortex()
{
    return hasVortex;
}

//Forces the player in a direction given by the parameter
void Player::forceDirection(int d)
{
    setMoveDirection(d);
    if (d == left)
        setDirection(left);
    else setDirection(right);
}

int Player::dieRoll()
{
    return getTicks()/8;
}

//if Player is waiting, and the action is fire, shoot the vortex
//if Player is waiting, and the action is Roll, roll the die
//if the Player is moving and is at a Fork, choose a new direction and keep moving until the die roll finished
void Player::doSomething()
{
if (isWaiting())
{
    int input = getWorld()->getAction(m_playerNum); //storing the getAction of the players in input variable
    if (input==ACTION_FIRE)
    {
        if (hasVortex==true) //if the Player has a Vortex
        {
            getWorld()->playSound(SOUND_PLAYER_FIRE);
            getWorld()->shootVortex(this);
            giveVortex(false);
        }
    }

    if (input==ACTION_ROLL) //dieroll action
    {
        die_roll = randInt(1, 10);
        setTicks(die_roll*8);
        setIsWaiting(false);
    }
    else return;
}
if (!isWaiting())
{
    if (getWorld()->isAtFork(this)) //Handles what a Player should do at Fork
    {
        int playerDirection = getWorld()->getAction(m_playerNum);
        if(playerDirection==ACTION_UP && getMoveDirection()!=down)
                setMoveDirection(up);
        else if(playerDirection==ACTION_RIGHT && getMoveDirection()!=left)
                setMoveDirection(right);
        else if(playerDirection==ACTION_DOWN && getMoveDirection()!=up)
                setMoveDirection(down);
        else if(playerDirection==ACTION_LEFT && getMoveDirection()!=right)
                setMoveDirection(left);
        else return;
    }
    keepMoving();
    minusTicks();
    if (getTicks()==0)
    {
        setIsWaiting(true);
    }
    setNewStatus(true);
}

}
void Player::addCoins(int c)
{
    coins = coins + c;
}
void Player::resetCoins()
{
    coins = 0; //resets Coins of a Player
}
void Player::setCoins(int c)
{
    coins = c;
}
void Player::setStars(int s)
{
    stars = s; //sets Stars of a Player to the value in the parameter
}
int Player::getCoins()
{
    return coins;
}
void Player::addStars(int s)
{
stars = stars + s;
}
int Player::getStars()
{
return stars;
}

//PEACH AND YOSHI CONSTRUCTORS
Peach::Peach(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Player(m, imageID, startX, startY, dir, depth, size, 1)
{}
Peach::~Peach(){}

Yoshi::Yoshi(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Player(m, imageID, startX, startY, dir, depth, size, 2)
{}
Yoshi::~Yoshi(){}

//SQUARE METHODS//////////////////////////////SQUARE METHODS////////////////////////////
//SQUARE METHODS//////////////////////////////SQUARE METHODS////////////////////////////
Square::Square(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Actor(m, imageID, startX, startY, dir, depth, size)
{}

Square::~Square(){}

void Square::doSomething(){}

//override Actor's isSquare to return true;
bool Square::isSquare()
{
    return true;
}

//COINSQUARE/////////////COINSQUARE/////////////COINSQUARE///////////
//COINSQUARE/////////////COINSQUARE/////////////COINSQUARE/////////////COINSQUARE///////////
CoinSquare::CoinSquare(bool color, StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Square(m, imageID, startX, startY, dir, depth, size)
{
    isBlue = color; //sets isBlue to the value passed in by the Parameter: either it is a blue coinsquare or not
}

CoinSquare::~CoinSquare() {}
bool CoinSquare::getColor()
{
    return isBlue;
}

//If a player lands on a coinsquare and it is blue, they get 3 coins.
//Otherwise the player lands on a red coin square, and loses 3 coins.
void CoinSquare::doSomething()
{
if (!getAlive()) //dont do anything for dead squares
    return;

for (int i = 1; i <=2; i++) //loops through both players
{
    if (getWorld()->getPlayer(i)->isWaiting() && getWorld()->getPlayer(i)->getNewStatus())
    {
        if (getWorld()->overlap(this, getWorld()->getPlayer(i)))
        {
            if (isBlue) //adds coins if Player is at a blue CoinSquare
            {
                getWorld()->getPlayer(i)->addCoins(3);
                getWorld()->getPlayer(i)->setNewStatus(false);
                getWorld()->playSound(SOUND_GIVE_COIN);
            }
            else //now cases for then the Player is at a red CoinSquare
            {
                if (getWorld()->getPlayer(i)->getCoins()<3)
                {
                    getWorld()->getPlayer(i)->setNewStatus(false);
                    getWorld()->getPlayer(i)->addCoins(-getWorld()->getPlayer(i)->getCoins());
                }
                else
                {
                    getWorld()->getPlayer(i)->addCoins(-3);
                    getWorld()->getPlayer(i)->setNewStatus(false);
                    getWorld()->playSound(SOUND_TAKE_COIN);
                }
            }
        }
    }
}
}

//STARSQUARE///////////STARSQUARE///////////STARSQUARE///////////STARSQUARE///////////STARSQUARE///////////STARSQUARE/////////
//STARSQUARE///////////STARSQUARE///////////STARSQUARE///////////STARSQUARE///////////STARSQUARE///////////STARSQUARE/////////
StarSquare::StarSquare(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Square(m, imageID, startX, startY, dir, depth, size)
{}

StarSquare::~StarSquare(){}

//if a Player lands on a StarSquare and has 20 coins, converts those 20 coins to 1 star
void StarSquare::doSomething()
{
if (!getAlive()) //dont do anything for dead squares
    return;

for (int i = 1; i<=2; i++)
{
    if (getWorld()->getPlayer(i)->isWaiting() && getWorld()->getPlayer(i)->getNewStatus())
    {
        if (getWorld()->overlap(this, getWorld()->getPlayer(i)))
        {
            if (getWorld()->getPlayer(i)->getCoins()<20) //case for when player does not have 20 coins
                return;
            else
            {
                getWorld()->getPlayer(i)->addCoins(-20); //subtracting 20 coins and adding a star if has at least 20 coins
                getWorld()->getPlayer(i)->addStars(1);
                getWorld()->getPlayer(i)->setNewStatus(false);
                getWorld()->playSound(SOUND_GIVE_STAR);
            }
        }
    }
}
}

//DIRECTIONALSQUARE///////DIRECTIONALSQUARE//////DIRECTIONALSQUARE//////DIRECTIONALSQUARE////
//DIRECTIONALSQUARE///////DIRECTIONALSQUARE//////DIRECTIONALSQUARE//////DIRECTIONALSQUARE////
DirectionalSquare::DirectionalSquare(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Square(m, imageID, startX, startY, dir, depth, size)
{}
DirectionalSquare::~DirectionalSquare(){}

bool DirectionalSquare::isDirectionalSquare()
{
return true;
}

//changes player Direction based on the direction of the DirectionalSquare
void DirectionalSquare::doSomething()
{
if (!getAlive()) //dont do anything for dead squares
    return;

for (int i = 1; i <=2; i++) //looping through the Players
{
    if (getWorld()->getPlayer(i)->getNewStatus())
    {
        if (getWorld()->overlap(this, getWorld()->getPlayer(i)))
        {
            getWorld()->getPlayer(i)->forceDirection(getDirection()); //changes player direction by calling forceDirection()
            getWorld()->getPlayer(i)->setNewStatus(false); //set new status to false
        }
    }
}
}

//BANKSQUARE///////////BANKSQUARE///////////BANKSQUARE///////////BANKSQUARE///////////BANKSQUARE/////////
//BANKSQUARE///////////BANKSQUARE///////////BANKSQUARE///////////BANKSQUARE///////////BANKSQUARE/////////
BankSquare::BankSquare(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Square(m, imageID, startX, startY, dir, depth, size)
{}

BankSquare::~BankSquare() {}

//if landing on this square, get all the money in the central bank.
//if passing through the BankSquare, donate 5 coins to central bank.
void BankSquare::doSomething()
{
if (!getAlive()) //dont do anything for dead squares
    return;

for (int i = 1; i <=2; i++)
{
    if (getWorld()->getPlayer(i)->getNewStatus()) //case for when it passes through the BankSquare
    {
        if (getWorld()->overlap(this, getWorld()->getPlayer(i)))
        {
            if (getWorld()->getPlayer(i)->getCoins()<5)
            {
                getWorld()->addToBank(getWorld()->getPlayer(i)->getCoins()); //add coins to bank
                getWorld()->getPlayer(i)->resetCoins();
                getWorld()->playSound(SOUND_DEPOSIT_BANK);
            }
            else{
                getWorld()->addToBank(5); //adding coins to bank
                getWorld()->getPlayer(i)->addCoins(-5);
                getWorld()->playSound(SOUND_DEPOSIT_BANK);
            }
        }
    }
    if (getWorld()->getPlayer(i)->isWaiting() && getWorld()->getPlayer(i)->getNewStatus()) //case for when player lands on BankSquare
    {
        if (getWorld()->overlap(this, getWorld()->getPlayer(i)))
        {
            getWorld()->getPlayer(i)->addCoins(getWorld()->getBank());
            getWorld()->resetBank(); //sets cental bank balance to 0 after player takes all coins from it
            getWorld()->getPlayer(i)->setNewStatus(false);
            getWorld()->playSound(SOUND_WITHDRAW_BANK);
        }
    }
}
}

//EVENTSQUARE///////////EVENTSQUARE//EVENTSQUARE//EVENTSQUARE//EVENTSQUARE//EVENTSQUARE
//EVENTSQUARE///////////EVENTSQUARE//EVENTSQUARE//EVENTSQUARE//EVENTSQUARE//EVENTSQUARE
EventSquare::EventSquare(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Square(m, imageID, startX, startY, dir, depth, size)
{}

EventSquare::~EventSquare(){}

//Randomly chooses between the 3 possible EventSquares and applies it to Players that land on it
void EventSquare::doSomething()
{
    if (!getAlive()) //dont do anything for dead squares
        return;

for (int i = 1; i <=2; i++)
{
    if (getWorld()->getPlayer(i)->isWaiting() && getWorld()->getPlayer(i)->getNewStatus())
    {
        int option = randInt(1, 3); //generating random number for the 3 different cases of EventSquare
        
        if (getWorld()->overlap(this, getWorld()->getPlayer(i)))
        {
            if (option ==1) //Randomly teleport Player to somewhere on screen
            {
                //function in StudentWorld that gets randomsquare, and returns those coordinates
                Actor* temp = getWorld()->getRandomActor();
                int randX = temp->getX();
                int randY = temp->getY();
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                getWorld()->getPlayer(i)->moveTo(randX, randY);
                getWorld()->getPlayer(i)->setNewStatus(true);
                getWorld()->getPlayer(i)->goRandomDirection();
            }
            if (option ==2) //Option to swap Yoshi and Peach
            {
                //swapping Yoshi and Peach by getting their coordinates, and moving them to each others
                int yoshiX = getWorld()->getYoshi()->getX();
                int yoshiY = getWorld()->getYoshi()->getY();
                int peachX = getWorld()->getPeach()->getX();
                int peachY = getWorld()->getPeach()->getY();
                int yoshiTicks = getWorld()->getYoshi()->getTicks();
                int peachTicks = getWorld()->getPeach()->getTicks();
                int yoshiDirection = getWorld()->getYoshi()->getMoveDirection();
                int peachDirection = getWorld()->getPeach()->getMoveDirection();
                bool yoshiIsWaiting = getWorld()->getYoshi()->isWaiting();
                bool peachIsWaiting = getWorld()->getPeach()->isWaiting();
                //now changing them for Yoshi
                getWorld()->getYoshi()->moveTo(peachX, peachY);
                getWorld()->getYoshi()->setTicks(peachTicks);
                getWorld()->getYoshi()->setMoveDirection(peachDirection); //also changes sprite direction
                getWorld()->getYoshi()->setIsWaiting(peachIsWaiting);
                //changing for Peach
                getWorld()->getPeach()->moveTo(yoshiX, yoshiY);
                getWorld()->getPeach()->setTicks(yoshiTicks);
                getWorld()->getPeach()->setMoveDirection(yoshiDirection);
                getWorld()->getPeach()->setIsWaiting(yoshiIsWaiting);
                
                getWorld()->getPlayer(i)->setNewStatus(false);
                
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            }
            if (option ==3) //option to give Player Vortex
            {
                //play sound and change has vortex
                getWorld()->getPlayer(i)->giveVortex(true);
                getWorld()->getPlayer(i)->setNewStatus(false);
                getWorld()->playSound(SOUND_GIVE_VORTEX);
            }
        }
    }
}
}

//DROPPINGSQUARE///////////DROPPINGSQUARE///////////DROPPINGSQUARE///////////DROPPINGSQUARE/////////
//DROPPINGSQUARE///////////DROPPINGSQUARE///////////DROPPINGSQUARE///////////DROPPINGSQUARE/////////
DroppingSquare::DroppingSquare(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Square(m, imageID, startX, startY, dir, depth, size)
{}
DroppingSquare::~DroppingSquare(){}

//Randomly chooses between the 2 possible DroppingSqures and applies it to Players that land on it
void DroppingSquare::doSomething()
{
for (int i = 1; i <=2; i++)
{
    if (getWorld()->getPlayer(i)->isWaiting() && getWorld()->getPlayer(i)->getNewStatus())
    {
        if (getWorld()->overlap(this, getWorld()->getPlayer(i)))
        {
            int option = randInt(1, 2); //randomly generates number to represent the options
            if (option ==1) //option 1 take away max 10 coins
            {
                if (getWorld()->getPlayer(i)->getCoins()<10)
                    getWorld()->getPlayer(i)->resetCoins();
                
                else getWorld()->getPlayer(i)->addCoins(-10);
                getWorld()->getPlayer(i)->setNewStatus(false);
                
            }
            if (option ==2) //option 2 take away a star if possible
            {
                if(getWorld()->getPlayer(i)->getStars()>0)
                    getWorld()->getPlayer(i)->addStars(-1);
                getWorld()->getPlayer(i)->setNewStatus(false);
            }
            getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
        }
    }
}
}

//BADDIE////////BADDIE////////BADDIE////////BADDIE////////BADDIE////////BADDIE////////BADDIE//////
//BADDIE////////BADDIE////////BADDIE////////BADDIE////////BADDIE////////BADDIE////////BADDIE//////
Baddie::Baddie(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Actor(m, imageID, startX, startY, dir, depth, size)
{
    baddie_counter = 180; //default Baddie counter
}
Baddie::~Baddie(){}

void Baddie::doSomething(){}

void Baddie::decrementBaddieCounter()
{
    baddie_counter--;
}

int Baddie::getBaddieCounter()
{
    return baddie_counter;
}
void Baddie::setBaddieCounter(int s)
{
    baddie_counter = s;
}
//overrides the Actor function that canBeVortexed() and sets it to true
bool Baddie::canBeVortexed()
{
    return true;
}
//if hit by a vortex, setAliveStatus to false (dead)
void Baddie::hit_by_vortex()
{
    setAliveStatus(false);
}

//BOWSER/////BOWSER//BOWSER/////BOWSER//BOWSER/////BOWSER//BOWSER/////BOWSER//BOWSER/////BOWSER
//BOWSER/////BOWSER//BOWSER/////BOWSER//BOWSER/////BOWSER//BOWSER/////BOWSER//BOWSER/////BOWSER
Bowser::Bowser(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Baddie(m, imageID, startX, startY, dir, depth, size)
{}

Bowser::~Bowser(){}

//if the Bowser is waiting and overlaps with a Player, there is a 50% chance Bowser takes all of the players coins
//if the Bowser is moving, keep moving and adjust baddieCounter (the amount to pause) and ticks accordingly
//Drops a Dropping Square 25% of the time
void Bowser::doSomething()
{
    if (isWaiting())
    {
        int rand = randInt(1, 1);
        for (int i = 1; i <=2; i++)
        {
            if (getWorld()->overlap(this, getWorld()->getPlayer(i)) && getWorld()->getPlayer(i)->isWaiting() && getWorld()->getPlayer(i)->getNewStatus() )
            {
                if (rand == 1)//case to take away half of player's coins;
                {
                    getWorld()->getPlayer(i)->resetCoins(); 
                    getWorld()->getPlayer(i)->setNewStatus(false);
                    getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
                }
            }
        }
        decrementBaddieCounter();
        if (getBaddieCounter()==0)
        {
            int squares_to_move = randInt(1, 10);
            setTicks(squares_to_move*8);
            setIsWaiting(false);
            goRandomDirection(); //makes Bowser go in a random direction
        }
    }
    if (!isWaiting())
    {
        if (getWorld()->isAtFork(this))
            goRandomDirection(); //goes in a random direction at Forks
        keepMoving();
    }
    minusTicks();
    if (getTicks()==0)
    {
        setIsWaiting(true);
        setBaddieCounter(180);
        int randDrop = randInt(1, 4); //drops a DroppingSquare 1/4 of the time
        if (randDrop == 1)
        {
            getWorld()->insertDroppingSquare(this); //drops DroppingSquare and replaces the original square by calling this function
            getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
        }
    }
}

//BOO///////BOO///////BOO///////BOO///////BOO///////BOO///////BOO///////BOO///////BOO/////
//BOO///////BOO///////BOO///////BOO///////BOO///////BOO///////BOO///////BOO///////BOO/////
Boo::Boo(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Baddie(m, imageID, startX, startY, dir, depth, size)
{}

Boo::~Boo(){}

//randomly swaps coins or stars when Player lands on a square with a paused Boo
void Boo::doSomething()
{
    if (isWaiting())
    {
        int rand = randInt(1, 2); //to decide whether Boo swaps Stars or Coins
        for (int i = 1; i <=2; i++)
        {
            if (getWorld()->overlap(this, getWorld()->getPlayer(i)) && getWorld()->getPlayer(i)->isWaiting() && getWorld()->getPlayer(i)->getNewStatus() )
            {
                getWorld()->playSound(SOUND_BOO_ACTIVATE);
                if (rand == 1) //case 1 where Boo swaps Coins
                {
                    int tempCoins = 0;
                    tempCoins = getWorld()->getPeach()->getCoins();
                    getWorld()->getPeach()->setCoins(getWorld()->getYoshi()->getCoins());
                    getWorld()->getYoshi()->setCoins(tempCoins);
                    getWorld()->playSound(SOUND_BOO_ACTIVATE);
                    getWorld()->getPlayer(i)->setNewStatus(false);
                }
                else if (rand == 2) // case 2 where Boo swaps Stars
                {
                    int tempStars = 0;
                    tempStars = getWorld()->getPeach()->getStars();
                    getWorld()->getPeach()->setStars(getWorld()->getYoshi()->getStars());
                    getWorld()->getYoshi()->setStars(tempStars);
                    getWorld()->playSound(SOUND_BOO_ACTIVATE);
                    getWorld()->getPlayer(i)->setNewStatus(false);
                }
            }
        }
        decrementBaddieCounter();
        if (getBaddieCounter()==0)
        {
            int squares_to_move = randInt(1, 3);
            setTicks(squares_to_move*8);
            setIsWaiting(false);
            goRandomDirection(); //go in a random direction every time Boo moves
        }
    }
    if (!isWaiting())
    {
        if (getWorld()->isAtFork(this))
            goRandomDirection(); //go in a random direction at Forks
        keepMoving();
    }
    minusTicks();
    if (getTicks()==0)
    {
        setIsWaiting(true);
        setBaddieCounter(180);
    }
}

//VORTEX///////////VORTEX///////////VORTEX///////////VORTEX///////////VORTEX///////////VORTEX/////////
//VORTEX///////////VORTEX///////////VORTEX///////////VORTEX///////////VORTEX///////////VORTEX/////////
Vortex::Vortex(StudentWorld*m, int imageID, int startX, int startY, int dir, int depth, double size): Actor(m, imageID, startX, startY, dir, depth, size)
{
    direction = dir;
}

Vortex::~Vortex(){}

//moves the Vortex depending on the direction which it is launched
void Vortex::doSomething()
{
    if (!getAlive())
        return; //donothing with dead Vortexes
    switch(direction)
    {
        case right:
        {
            moveTo(getX()+2, getY());
            break;
        }
        case left:
        {
            moveTo(getX()-2, getY());
            break;
        }
        case up:
        {
            moveTo(getX(), getY()+2);
            break;
        }
        case down:
        {
            moveTo(getX(), getY()-2);
            break;
        }
    }
    
    //relocates Baddies when they are touched by Vortex
    getWorld()->vortexTouches(this);
    
    //deletes the vortex if it goes off the screen.
    if (getX() > VIEW_WIDTH + SPRITE_WIDTH || getY()> VIEW_HEIGHT + SPRITE_WIDTH)
    {
        setAliveStatus(false);
    }
}


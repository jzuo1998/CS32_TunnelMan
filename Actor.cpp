#include "Actor.h"
#include "StudentWorld.h"
//////////////////////////////////////
///////Actor//////////////////////////
//////////////////////////////////////
Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth,StudentWorld* world): GraphObject(imageID, startX, startY, dir, size, depth)
{
    studentworld=world;
    setAlive();
}
Actor::~Actor(){
    setVisible(false);
}
//void Actor::doSomething()=0;
StudentWorld* Actor::getWorld()
{
    return studentworld;
}
bool Actor::isAlive()
{
    return m_alive;
}
void Actor::setAlive()
{
    m_alive=true;
}
void Actor::setDead()
{
    m_alive=false;
}
void Actor::pickupOil(){}
void Actor::pickupSonar(){}
void Actor::isAnnoyed(){}
void Actor::hitByBoulder(){}
void Actor::pickupGold(){}

//////////////////////////////////////
///////Tunnelman//////////////////////
//////////////////////////////////////

Tunnelman::Tunnelman(StudentWorld* studentworld): Actor(TID_PLAYER, 30, 60, right, 1.0, 0,studentworld),m_water(5),m_sonar(1),m_gold(0),m_health(100),m_oil(0)
{
    setVisible(true);
}
Tunnelman::~Tunnelman(){setVisible(false);}
void Tunnelman::deHealth(int amt)
{
    m_health-=amt;
}
int Tunnelman::getHealth() {return m_health;}
void Tunnelman::isAnnoyed()
{
    deHealth(20);
    if(getHealth()<=0)
    {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        return;
    }
    getWorld()->playSound(SOUND_PLAYER_ANNOYED);
}
void Tunnelman::pickupGold()
{
    m_gold++;
}
void Tunnelman::pickupOil() 
{
    m_oil++;
}
void Tunnelman::pickupWater()
{
    m_water+=5;
}
int Tunnelman::getWater(){return m_water;}
int Tunnelman::getSonar(){return m_sonar;}
int Tunnelman::getGold(){return m_gold;}
int Tunnelman::getOil() {return m_oil;}
void Tunnelman::squirt(Direction dir){}
void Tunnelman::hitByBoulder()
{
    setDead();
    setVisible(false);
    getWorld()->decLives();
    getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
}
void Tunnelman::pickupSonar()
{
    m_sonar++;
}
void Tunnelman::useSonar(int x, int y){}
void Tunnelman::doSomething()
{
    if(isAlive()==false)
        return;
    //getWorld()->removeEarth(getX(), getY());
    int i;
    if(getWorld()->getKey(i))
    {
        switch(i)
        {
            case KEY_PRESS_UP:
                if(getDirection()==up && getWorld()->canMove(getX(), getY()+1))
                {
                    if(getX()<=60 && getY()<60)
                    {
                        moveTo(getX(), getY()+1);
                        getWorld()->removeEarth(getX(), getY());
                    }
                }
                else
                    setDirection(up);
                break;
            case KEY_PRESS_DOWN:
                if(getDirection()==down && getWorld()->canMove(getX(), getY()-1))
                {
                    if(getY()>=1)
                    {
                        moveTo(getX(), getY()-1);
                        getWorld()->removeEarth(getX(), getY());
                    }
                }
                else
                    setDirection(down);
                break;
            case KEY_PRESS_LEFT:
                if(getDirection()==left && getWorld()->canMove(getX()-1, getY()))
                {
                    if(getX()>=1)
                    {
                        moveTo(getX()-1, getY());
                        getWorld()->removeEarth(getX(), getY());
                    }
                }
                else
                    setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                if(getDirection()==right && getWorld()->canMove(getX()+1, getY()))
                {
                    if(getX()<60)
                    {
                        moveTo(getX()+1, getY());
                        getWorld()->removeEarth(getX(), getY());
                    }
                }
                else
                    setDirection(right);
                break;
            case KEY_PRESS_ESCAPE:
                setDead();
                break;
            case KEY_PRESS_TAB:
                if(m_gold>0)
                {
                    m_gold--;
                    getWorld()->dropGold(getX(), getY());
                }
                break;
            case KEY_PRESS_SPACE:
                if(m_water>0)
                {
                    m_water--;
                    switch(getDirection())
                    {
                        case up:
                            getWorld()->addsquirt(getX(), getY()+4, up);
                            break;
                        case down:
                            getWorld()->addsquirt(getX(), getY()-4, down);
                            break;
                        case right:
                            getWorld()->addsquirt(getX()+4, getY(), right);
                            break;
                        case left:
                            getWorld()->addsquirt(getX()-4, getY(), left);
                            break;
                        default:
                            break;
                            
                    }
                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                    break;
                }
                else
                    break;
            case 'z':
            case 'Z':            //use sonar kit
                if (m_sonar>0)
                {
                    m_sonar--;
                    getWorld()->NearbyObject(getX(), getY(), 12);
                    getWorld()->playSound(SOUND_SONAR);
                }
                break;
            default:
                break;
                
        }
    }
    
}
//////////////////////////////////////
///////Protester//////////////////////
//////////////////////////////////////
Protester::Protester(int IMID, StudentWorld* studentworld):Actor(IMID, 60, 60, left, 1.0, 0,studentworld)
{
    ticks=fmax(0, 3- getWorld()->getLevel()/4);
    m_leave=false;
}
Protester::~Protester()
{
    setVisible(false);
}
int Protester::getTicks()
{
    return ticks;
}
void Protester::setTicks(int n)
{
    ticks=n;
}
void Protester::setLeave()
{
    m_leave=true;
}
void Protester::pickupGold(){}
void Protester::isAnnoyed(){}
void Protester::hitByBoulder(){}
bool Protester::leave()
{
    return m_leave;
}

RegularProtester::RegularProtester(StudentWorld* studentworld):Protester(TID_PROTESTER, studentworld)
{
    setVisible(true);
    shout=0;
    turn=0;
    numSquaresToMove=rand()%53+8;
    m_health=5;
}
RegularProtester::~RegularProtester(){setVisible(false);}
void RegularProtester::pickupGold()
{
    //getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    setLeave();
    getWorld()->getPath(getX(), getY(),solution);
}
void RegularProtester::isAnnoyed()
{
    if(leave())
        return;
    m_health-=2;
    if(m_health<=0)
    {
        setLeave();
        getWorld()->getPath(getX(), getY(),solution);
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        setTicks(0);
        getWorld()->increaseScore(100);
    }
    else
    {
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        setTicks(fmax(50, 100-getWorld()->getLevel()*10));
    }
    
}
void RegularProtester::doSomething()
{
    if(!isAlive())
        return;
    if(getTicks()==0)
    {
        setTicks(fmax(0, 3-getWorld()->getLevel()/4));
    }
    else
    {
        setTicks(getTicks()-1);
        return;
    }
    if(leave()==true)
    {
        if(getX()==60 && getY()==60)
        {
            setDead();
            setVisible(false);
            return;
        }
        else
        {
            int x,y;
            //getWorld()->getPath(getX(), getY(),solution);
            if(!solution.empty())
            {
                x=solution.top().first;
                y=solution.top().second;
                solution.pop();
                if(getY()==y)
                {
                    if(x<getX())
                    {
                        setDirection(left);
                        moveTo(x, y);
                        return;
                    }
                    else
                    {
                        setDirection(right);
                        moveTo(x, y);
                        return;
                    }
                }
                if(getX()==x)
                {
                    if(y<getY())
                    {
                        setDirection(down);
                        moveTo(x, y);
                        return;
                    }
                    else
                    {
                        setDirection(up);
                        moveTo(x, y);
                        return;
                    }
                }
            }
            //return;
        }
    }
    if(getWorld()->closeToTunnelman(getX(), getY(), 4) && getWorld()->facingTunnelman(getX(), getY(), getDirection()))
    {
        if(shout<=0)
        {
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            getWorld()->shout();
            shout=15;
            return;
        }
    }
    else
        shout--;
    if(getWorld()->straightline(getX(), getY())!=none
       && getWorld()->closeToTunnelman(getX(), getY(), 4)==false)
    {
        setDirection(getWorld()->straightline(getX(), getY()));
        numSquaresToMove=0;
        switch(getWorld()->straightline(getX(), getY()))
        {
            case up:
                moveTo(getX(), getY()+1);
                return;
                break;
            case down:
                moveTo(getX(), getY()-1);
                return;
                break;
            case left:
                moveTo(getX()-1, getY());
                return;
                break;
            case right:
                moveTo(getX()+1, getY());
                return;
                break;
            default:
                break;
        }
        return;
    }
    else
    {
        numSquaresToMove--;
        Direction dir=none;
        if(numSquaresToMove<=0)
        {
            do
            {
                int temp=rand()%4;
                switch(temp)
                {
                    case 0:
                        dir=up;
                        break;
                    case 1:
                        dir=down;
                        break;
                    case 2:
                        dir=left;
                        break;
                    case 3:
                        dir=right;
                        break;
                    default:
                        break;
                }
            }while(!getWorld()->canMove(getX(), getY(),dir));
            setDirection(dir);
            int temp=rand()%53+8;
            numSquaresToMove=temp;
        }
        else if(getWorld()->intersection(getX(), getY(), getDirection()))
        {
            if(turn<=0)
            {
                if(dir==GraphObject::up ||dir==GraphObject::down)
                {
                    if(getWorld()->canMove(getX(), getY(),GraphObject::left) && getWorld()->canMove(getX(), getY(),GraphObject::right))
                    {
                        int num=rand()%2;
                        if(num==0)
                            setDirection(left);
                        else
                            setDirection(right);
                    }
                    else if(getWorld()->canMove(getX(), getY(),GraphObject::left))
                        setDirection(left);
                    else if(getWorld()->canMove(getX(), getY(),GraphObject::right))
                        setDirection(right);
                    else
                    {
                        if(getWorld()->canMove(getX(), getY(),GraphObject::up))
                            setDirection(up);
                        else
                            setDirection(down);
                    }
                    
                }
                if(dir==GraphObject::right ||dir==GraphObject::left)
                {
                    if(getWorld()->canMove(getX(), getY(),GraphObject::up) && getWorld()->canMove(getX(), getY(),GraphObject::down))
                    {
                        int num=rand()%2;
                        if(num==0)
                            setDirection(up);
                        else
                            setDirection(down);
                    }
                    else if(getWorld()->canMove(getX(), getY(),GraphObject::up))
                        setDirection(up);
                    else if(getWorld()->canMove(getX(), getY(),GraphObject::down))
                        setDirection(down);
                    else
                    {
                        if(getWorld()->canMove(getX(), getY(),GraphObject::right))
                            setDirection(right);
                        else
                            setDirection(left);
                    }
                    
                }
                numSquaresToMove=rand()%53+8;
                turn=200;
            }
            
        }
        
    }
    if(getWorld()->canMove(getX(), getY(), getDirection()))
    {
        switch(getDirection())
        {
            case GraphObject::up:
                moveTo(getX(), getY()+1);
                break;
            case GraphObject::down:
                moveTo(getX(), getY()-1);
                break;
            case GraphObject::left:
                moveTo(getX()-1, getY());
                break;
            case GraphObject::right:
                moveTo(getX()+1, getY());
                break;
            default:
                break;
                
        }
    }
    else
        numSquaresToMove=0;
    turn--;
    
}
void RegularProtester::hitByBoulder()
{
    setLeave();
    getWorld()->increaseScore(500);
    getWorld()->getPath(getX(), getY(), solution);
}

HardcoreProtester::HardcoreProtester(StudentWorld* studentworld):Protester(TID_HARD_CORE_PROTESTER, studentworld)
{
    setVisible(true);
    shout=0;
    turn=0;
    numSquaresToMove=rand()%53+8;
    m_health=20;
    stare=0;
}
HardcoreProtester::~HardcoreProtester(){setVisible(false);}
void HardcoreProtester::pickupGold()
{
    getWorld()->increaseScore(50);
    setTicks(fmax(50, 100-getWorld()->getLevel()* 10));
}
void HardcoreProtester::doSomething()
{
    if(!isAlive())
        return;
    if(getTicks()==0)
    {
        setTicks(fmax(0, 3-getWorld()->getLevel()/4));
    }
    else
    {
        setTicks(getTicks()-1);
        return;
    }
    if(leave()==true)
    {
        if(getX()==60 && getY()==60)
        {
            setDead();
            setVisible(false);
            return;
        }
        else
        {
            int x,y;
            //getWorld()->getPath(getX(), getY(),solution);
            if(!solution.empty())
            {
                x=solution.top().first;
                y=solution.top().second;
                solution.pop();
                if(getY()==y)
                {
                    if(x<getX())
                    {
                        setDirection(left);
                        moveTo(x, y);
                        return;
                    }
                    else
                    {
                        setDirection(right);
                        moveTo(x, y);
                        return;
                    }
                }
                if(getX()==x)
                {
                    if(y<getY())
                    {
                        setDirection(down);
                        moveTo(x, y);
                        return;
                    }
                    else
                    {
                        setDirection(up);
                        moveTo(x, y);
                        return;
                    }
                }
            }
            //return;
        }
    }
    if(getWorld()->closeToTunnelman(getX(), getY(), 4) && getWorld()->facingTunnelman(getX(), getY(), getDirection()))
    {
        if(shout<=0)
        {
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            getWorld()->shout();
            shout=15;
            return;
        }
    }
    else
        shout--;
    if(getWorld()->closeToTunnelman(getX(), getY(), 4)==false)
    {
        int M=16 + getWorld()->getLevel()* 2;
        std::stack<std::pair<int, int>> pathtoTunnelman;
        getWorld()->getPathtoTunnelman(getX(), getY(), pathtoTunnelman);
        if(pathtoTunnelman.size()<=M)
        {
            int x,y;
            if(!pathtoTunnelman.empty())
            {
                x=pathtoTunnelman.top().first;
                y=pathtoTunnelman.top().second;
                pathtoTunnelman.pop();
                if(getY()==y)
                {
                    if(x<getX())
                    {
                        setDirection(left);
                        moveTo(x, y);
                        return;
                    }
                    else
                    {
                        setDirection(right);
                        moveTo(x, y);
                        return;
                    }
                }
                if(getX()==x)
                {
                    if(y<getY())
                    {
                        setDirection(down);
                        moveTo(x, y);
                        return;
                    }
                    else
                    {
                        setDirection(up);
                        moveTo(x, y);
                        return;
                    }
                }
            }
        }
        if(getWorld()->straightline(getX(), getY())!=none)
        {
            setDirection(getWorld()->straightline(getX(), getY()));
            numSquaresToMove=0;
            switch(getWorld()->straightline(getX(), getY()))
            {
                case up:
                    moveTo(getX(), getY()+1);
                    return;
                    break;
                case down:
                    moveTo(getX(), getY()-1);
                    return;
                    break;
                case left:
                    moveTo(getX()-1, getY());
                    return;
                    break;
                case right:
                    moveTo(getX()+1, getY());
                    return;
                    break;
                default:
                    break;
            }
            return;
        }
        else
        {
            numSquaresToMove--;
            Direction dir=none;
            if(numSquaresToMove<=0)
            {
                do
                {
                    int temp=rand()%4;
                    switch(temp)
                    {
                        case 0:
                            dir=up;
                            break;
                        case 1:
                            dir=down;
                            break;
                        case 2:
                            dir=left;
                            break;
                        case 3:
                            dir=right;
                            break;
                        default:
                            break;
                    }
                }while(!getWorld()->canMove(getX(), getY(),dir));
                setDirection(dir);
                int temp=rand()%53+8;
                numSquaresToMove=temp;
            }
            else if(getWorld()->intersection(getX(), getY(), getDirection()))
            {
                if(turn<=0)
                {
                    if(dir==GraphObject::up ||dir==GraphObject::down)
                    {
                        if(getWorld()->canMove(getX(), getY(),GraphObject::left) && getWorld()->canMove(getX(), getY(),GraphObject::right))
                        {
                            int num=rand()%2;
                            if(num==0)
                                setDirection(left);
                            else
                                setDirection(right);
                        }
                        else if(getWorld()->canMove(getX(), getY(),GraphObject::left))
                            setDirection(left);
                        else if(getWorld()->canMove(getX(), getY(),GraphObject::right))
                            setDirection(right);
                        else
                        {
                            if(getWorld()->canMove(getX(), getY(),GraphObject::up))
                                setDirection(up);
                            else
                                setDirection(down);
                        }
                        
                    }
                    if(dir==GraphObject::right ||dir==GraphObject::left)
                    {
                        if(getWorld()->canMove(getX(), getY(),GraphObject::up) && getWorld()->canMove(getX(), getY(),GraphObject::down))
                        {
                            int num=rand()%2;
                            if(num==0)
                                setDirection(up);
                            else
                                setDirection(down);
                        }
                        else if(getWorld()->canMove(getX(), getY(),GraphObject::up))
                            setDirection(up);
                        else if(getWorld()->canMove(getX(), getY(),GraphObject::down))
                            setDirection(down);
                        else
                        {
                            if(getWorld()->canMove(getX(), getY(),GraphObject::right))
                                setDirection(right);
                            else
                                setDirection(left);
                        }
                        
                    }
                    numSquaresToMove=rand()%53+8;
                    turn=200;
                }
                
            }
            
        }
        if(getWorld()->canMove(getX(), getY(), getDirection()))
        {
            switch(getDirection())
            {
                case GraphObject::up:
                    moveTo(getX(), getY()+1);
                    break;
                case GraphObject::down:
                    moveTo(getX(), getY()-1);
                    break;
                case GraphObject::left:
                    moveTo(getX()-1, getY());
                    break;
                case GraphObject::right:
                    moveTo(getX()+1, getY());
                    break;
                default:
                    break;
                    
            }
        }
        else
            numSquaresToMove=0;
        turn--;
    }
    
}
void HardcoreProtester::isAnnoyed()
{
    if(leave())
        return;
    m_health-=2;
    if(m_health<=0)
    {
        setLeave();
        getWorld()->getPath(getX(), getY(),solution);
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        setTicks(0);
        getWorld()->increaseScore(250);
    }
    else
    {
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        setTicks(fmax(50, 100-getWorld()->getLevel()*10));
    }
}
void HardcoreProtester::hitByBoulder()
{
    setLeave();
    getWorld()->increaseScore(500);
    getWorld()->getPath(getX(), getY(), solution);
}
//void HardcoreProtester::findTunnelman(){}
//void HardcoreProtester::findPath(int x,int y){}
//////////////////////////////////////
///////Earth//////////////////////////
//////////////////////////////////////
Earth::Earth(int startX, int startY, StudentWorld* world):GraphObject(TID_EARTH, startX, startY, right, 0.25, 3)
{
    setVisible(true);
    studentworld=world;
}
Earth::~Earth()
{
    setVisible(false);
}
StudentWorld* Earth::getWorld()
{
    return studentworld;
}
//////////////////////////////////////
///////Boulder//////////////////////////
//////////////////////////////////////
Boulder::Boulder(int startX, int startY, StudentWorld* world):Actor(TID_BOULDER, startX, startY, down, 1, 1,world)
{
    setVisible(true);
    setAlive();
    falling=30;
    m_falling=false;
    setStable(true);
}
Boulder::~Boulder()
{
    setVisible(false);
    
}
bool Boulder::isStable(){return m_stable;}
void Boulder::setStable(bool stable)
{
    m_stable=stable;
}
void Boulder::doSomething()
{
    if(!isAlive())
        return;
    if(isStable())
    {
        if(getWorld()->noEarth(getX(), getY()-1))
        {
            setStable(false);
        }
        else
            return;
    }
    else
    {
        if(falling==0)
        {
            m_falling=true;
        }
        else
        {
            falling--;
            return;        }
    }
    if(m_falling==true)
    {
        if(getWorld()->noEarth(getX(), getY()-1))
        {
            moveTo(getX(), getY()-1);
            getWorld()->playSound(SOUND_FALLING_ROCK);
            getWorld()->fallingRock(getX(), getY());
        }
        else
        {
            setDead();
            setVisible(false);
        }
    }
}
//////////////////////////////////////
///////Squirt//////////////////////////
//////////////////////////////////////
Squirt::Squirt(int startX, int startY,Direction dir, StudentWorld* world): Actor(TID_WATER_SPURT, startX, startY, dir, 1, 1,world)
{
    setVisible(true);
    setAlive();
    dis=0;
}
Squirt::~Squirt(){setVisible(false);}
void Squirt::doSomething()
{
    if(getWorld()->squirtArround(getX(), getY()))
    {
        setDead();
    }
    if(dis==3)
    {
        setDead();
        setVisible(false);
    }
    else
    {
        dis++;
        switch (getDirection())
        {
            case left:
                if (!getWorld()->canMove(getX()-1, getY()) || !getWorld()->noEarth(getX()-1, getY()))
                {
                    setDead();
                    setVisible(false);
                }
                else
                    moveTo(getX()-1, getY());
                break;
            case right:
                if (!getWorld()->canMove(getX()+1, getY()) || !getWorld()->noEarth(getX()+1, getY()))
                {
                    setDead();
                    setVisible(false);
                }
                else
                    moveTo(getX()+1, getY());
                break;
            case down:
                if (!getWorld()->canMove(getX(), getY()-1) || !getWorld()->noEarth(getX(), getY()-1))
                {
                    setDead();
                    setVisible(false);
                }
                else
                    moveTo(getX(), getY()-1);
                break;
            case up:
                if (!getWorld()->canMove(getX(), getY()+1) || !getWorld()->noEarth(getX(), getY()+1))
                {
                    setDead();
                    setVisible(false);
                }
                else
                    moveTo(getX(), getY()+1);
                break;
            default:
                break;
        }
    }
}
void Squirt::playSound(){}
//////////////////////////////////////
///////Oil//////////////////////////
//////////////////////////////////////
BarrelOfOil::BarrelOfOil(int startX, int startY, StudentWorld* world):Actor(TID_BARREL,startX,startY,right,1.0,2,world)
{
    setAlive();
    m_visible=false;
}
BarrelOfOil::~BarrelOfOil(){setVisible(false);}
void BarrelOfOil::doSomething()
{
    if(!isAlive())
        return;
    if(m_visible==false && getWorld()->closeToTunnelman(getX(), getY(),4))
    {
        setVisible(true);
        //return;
    }
    if(getWorld()->closeToTunnelman(getX(), getY(),3))
    {
        setDead();
        setVisible(false);
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->pickupOil();
    }
}
//////////////////////////////////////
///////Gold//////////////////////////
//////////////////////////////////////
GoldNugget::GoldNugget(int startX, int startY, StudentWorld* world,bool visible,bool temp):Actor(TID_GOLD,startX,startY,right,1.0,2,world)
{
    setAlive();
    m_temp=temp;
    setVisible(visible);
    m_visible=visible;
    isBribe=visible;
    lifetime=100;
}
GoldNugget::~GoldNugget(){setVisible(false);}
void GoldNugget::doSomething()
{
    if(!isAlive())
        return;
    if(m_visible==false && getWorld()->closeToTunnelman(getX(), getY(),4))
    {
        setVisible(true);
        //return;
    }
    if(isBribe==false&& getWorld()->closeToTunnelman(getX(), getY(),3))
    {
        setDead();
        setVisible(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(10);
        getWorld()->pickupGold();
    }
    if(isBribe==true && getWorld()->closeToProtester(getX(), getY(), 3))
    {
        setDead();
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        //getWorld()->increaseScore(25);
    }
    if(m_temp==true)
    {
        if(lifetime==0)
            setDead();
        else
            lifetime--;
    }
}
//////////////////////////////////////
///////Sonar//////////////////////////
//////////////////////////////////////
SonarKit::SonarKit(StudentWorld* world):Actor(TID_SONAR,0, 60, right, 1.0, 2,world)
{
    //setAlive();
    setVisible(true);
    lifetime=fmax(100, 300-10*world->getLevel());
}
SonarKit::~SonarKit(){setVisible(false);}
void SonarKit::doSomething()
{
    if(!isAlive())
        return;
    if(getWorld()->closeToTunnelman(getX(), getY(), 3))
    {
        setDead();
        setVisible(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->pickupSonar();
        getWorld()->increaseScore(75);
    }
    if(lifetime==0)
    {
        setDead();
        setVisible(false);
        return;
    }
    lifetime--;
}
//////////////////////////////////////
///////Water//////////////////////////
//////////////////////////////////////
WaterPool::WaterPool(int x, int y, StudentWorld* world):Actor(TID_WATER_POOL,x, y, right, 1.0, 2,world)
{
    setVisible(true);
    lifetime=fmax(100, 300-10*world->getLevel());
}
WaterPool::~WaterPool()
{setVisible(false);}
void WaterPool::doSomething()
{
    if(!isAlive())
        return;
    if(getWorld()->closeToTunnelman(getX(), getY(), 3))
    {
        setDead();
        setVisible(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->pickupWater();
        getWorld()->increaseScore(75);
    }
    if(lifetime==0)
    {
        setDead();
        setVisible(false);
    }
    else
        lifetime--;
}
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

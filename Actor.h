#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include <stack>

//#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//include tunnelman and protester
class StudentWorld;
class Actor :public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth,StudentWorld* world);
    virtual ~Actor();
    virtual void doSomething()=0;
    StudentWorld* getWorld();
    bool isAlive();
    void setAlive();
    //void setMAlive(bool alive);
    virtual void isAnnoyed();
    void setDead();
    virtual void pickupSonar();
    virtual void pickupOil();
    virtual void hitByBoulder();
    virtual void pickupGold();
private:
    bool m_alive;
    StudentWorld* studentworld;
    
    
};
class Tunnelman :public Actor
{
public:
    Tunnelman(StudentWorld* studentworld);
    ~Tunnelman();
    void doSomething();
    void deHealth(int amt);
    int getHealth();
    void isAnnoyed();
    int getWater();
    int getSonar();
    void pickupGold();
    void pickupOil();
    void pickupSonar();
    void pickupWater();
    int getGold();
    int getOil();
    void squirt(Direction dir);
    void hitByBoulder();
    void useSonar(int x, int y);
private:
    int m_water;
    int m_sonar;
    int m_gold;
    int m_health;
    int m_oil;
};
class Protester: public Actor
{
public:
    Protester(int IMID, StudentWorld* studentworld);
    virtual ~Protester();
    int getTicks();
    void setTicks(int n);
    virtual void doSomething()=0;
    virtual void pickupGold();
    virtual void isAnnoyed();
    void setLeave();
    virtual void hitByBoulder();
    bool leave();
private:
    int ticks;
    bool m_leave;
    
};
class RegularProtester: public Protester
{
public:
    RegularProtester(StudentWorld* studentworld);
    ~RegularProtester();
    void pickupGold();
    void doSomething();
    void isAnnoyed();
    void hitByBoulder();
private:
    int shout;
    int turn;
    int numSquaresToMove;
    int m_health;
    std::stack<std::pair<int,int>> solution;
};
class HardcoreProtester: public Protester
{
public:
    HardcoreProtester(StudentWorld* studentworld);
    ~HardcoreProtester();
    void pickupGold();
    void doSomething();
    void hitByBoulder();
    void isAnnoyed();
    //void findTunnelman();
    //void findPath(int x,int y);
private:
    int stare;
    int shout;
    int turn;
    int numSquaresToMove;
    int m_health;
    std::stack<std::pair<int,int>> solution;
};
class Earth: public GraphObject
{
public:
    Earth(int startX, int startY, StudentWorld* world);
    ~Earth();
    StudentWorld* getWorld();
private:
    StudentWorld* studentworld;
};
class Boulder: public Actor
{
public:
    Boulder(int startX, int startY, StudentWorld* world);
    ~Boulder();
    bool isStable();
    void setStable(bool stable);
    void doSomething();
    //void drop();
private:
    bool m_stable;
    int falling;
    bool m_falling;
    
};
class Squirt: public Actor
{
public:
    Squirt(int startX, int startY,Direction dir, StudentWorld* world);
    ~Squirt();
    void doSomething();
    void playSound();
private:
    int dis;
};
class BarrelOfOil: public Actor
{
public:
    BarrelOfOil(int startX, int startY, StudentWorld* world);
    ~BarrelOfOil();
    void doSomething();
private:
    bool m_visible;
};
class GoldNugget: public Actor
{
public:
    GoldNugget(int startX, int startY, StudentWorld* world,bool visible,bool temp);
    ~GoldNugget();
    void doSomething();
    
private:
    bool m_temp;
    bool m_visible;
    bool isBribe;
    int lifetime;
};
class SonarKit : public Actor
{
public:
    SonarKit(StudentWorld* world);
    virtual ~SonarKit();
    virtual void doSomething();
    
private:
    int lifetime;
};
class WaterPool : public Actor
{
public:
    WaterPool(int x, int y, StudentWorld* world);
    virtual ~WaterPool();
    virtual void doSomething();
    private:
    int lifetime;
};
#endif // ACTOR_H_

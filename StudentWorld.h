#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include<iomanip>
#include "Actor.h"
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <iostream>
#include <queue>
#include <stack>
#include <cstdlib>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    bool canMove(int x, int y);
    bool noObjectNearby(int x,int y);
    bool noEarth(int x, int y);
    int getDisSquare(int x1, int y1, int x2,int y2);
    void removeEarth(int x,int y);
    void removeEarth2(int x,int y);
    void pickupGold();
    GraphObject::Direction nextMove(int x,int y);
    void pickupOil();
    void NearbyObject(int x, int y, int rad);
    void dropGold(int x, int y);
    void getPath(int x, int y,std::stack<std::pair<int, int>>& solution);
    void getPathtoTunnelman(int x, int y,std::stack<std::pair<int, int>>& solution);
    void addsquirt(int x,int y, Actor::Direction dir);
    void addNewProtester();
    void pickupSonar();
    void pickupWater();
    void shout();
    bool intersection(int x, int y,GraphObject::Direction dir);
    bool canMove(int x, int y, GraphObject::Direction dir);
    GraphObject::Direction straightline(int x, int y);
    bool facingTunnelman(int x, int y, GraphObject::Direction dir);
    bool closeToTunnelman(int x, int y, int n);
    bool closeToProtester(int x, int y, int n);
    void fallingRock(int x, int y);
    bool squirtArround(int x, int y);
    void upDateText();
    bool noWater(int x, int y);

    virtual int init();

    virtual int move();

    virtual void cleanUp();
	

private:
    struct Point
    {
        GraphObject::Direction dir;
        int x;
        int y;
    };
    Earth* m_earth[64][64];
    Tunnelman* m_tunnelman;
    bool firstTick;
    bool canVisit[61][61];
    bool Visited[61][61];
    //std::stack<std::pair<int, int>> solution;
    std::vector<Actor*> actors;
    int countTickP;
    bool water[61][61];
    std::vector<Actor*> gameObject;
};

#endif // STUDENTWORLD_H_

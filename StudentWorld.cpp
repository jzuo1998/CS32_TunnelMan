#include "StudentWorld.h"
#include <string>

using namespace std;
GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}
StudentWorld::StudentWorld(std::string assetDir): GameWorld(assetDir)
{
    countTickP=0;
    firstTick=true;
    m_tunnelman=nullptr;
}
StudentWorld::~StudentWorld()
{
    cleanUp();
}
bool StudentWorld::canMove(int x, int y)
{
    if(x<0 || x>60 ||y<0||y>60)
        return false;
    for(int i=x;i<x+4;i++)
    {
        for(int j=y;j<y+4;j++)
        {
            if(canVisit[i][j]==false)
            {
                return false;
            }
        }
    }
    return true;
}
bool StudentWorld::facingTunnelman(int x, int y, GraphObject::Direction dir)
{
    switch(dir)
    {
        case GraphObject::left:
        {
            if (m_tunnelman->getX()<x)
                return true;
            break;
        }
        case GraphObject::right:
        {
            if (m_tunnelman->getX()>x)
                return true;
            break;
        }
        case GraphObject::up:
        {
            if (m_tunnelman->getY()>y)
                return true;
            break;
        }
        case GraphObject::down:
        {
            if (m_tunnelman->getY()<y)
                return true;
            break;
        }
        default:
            break;
    }
    return false;
}
void StudentWorld::shout()
{
    m_tunnelman->isAnnoyed();
}
bool StudentWorld::canMove(int x, int y, GraphObject::Direction dir)
{
    switch(dir)
    {
        case GraphObject::up:
            return noEarth(x, y+1);
            break;
        case GraphObject::down:
            return noEarth(x, y-1);
            break;
        case GraphObject::left:
            return noEarth(x-1, y);
            break;
        case GraphObject::right:
            return noEarth(x+1, y);
            break;
        default:
            return false;
            break;
            
    }
}
GraphObject::Direction StudentWorld::straightline(int x, int y)
{
    
    if(m_tunnelman->getX()==x)
    {
        bool rt=true;
        for(int i=fmin(y,m_tunnelman->getY());i<fmax(y,m_tunnelman->getY());i++)
        {
            if(!noEarth(x, i) || canVisit[x][i]==false)
                rt=false;
        }
        if(rt==true)
        {
            if(m_tunnelman->getY()<y)
                return GraphObject::down;
            if(m_tunnelman->getY()>y)
                return GraphObject::up;
        }
    }
    
    if(m_tunnelman->getY()==y)
    {
        bool rt2=true;
        for(int i=fmin(x,m_tunnelman->getX());i<fmax(x,m_tunnelman->getX());i++)
        {
            if(!noEarth(i, y))
                rt2=false;
        }
        if(rt2==true)
        {
            if(m_tunnelman->getX()<x)
                return GraphObject::left;
            if(m_tunnelman->getX()>x)
                return GraphObject::right;
        }

    }
    return GraphObject::none;
}
bool StudentWorld::noObjectNearby(int x,int y)
{
    for(int i=0;i<gameObject.size();i++)
    {
        int disX=gameObject[i]->getX()-x;
        int disY=gameObject[i]->getY()-y;
        if((disX*disX+disY*disY)<=36)
        {
            return false;
        }
        
    }
    return true;
}
bool StudentWorld::noEarth(int x, int y)
{
    if(x<0 || y<0 ||x>60 ||y>60)
        return false;
    for(int i=x;i<x+4;i++)
    {
        for(int j=y;j<y+4;j++)
        {
            if(m_earth[i][j]!=nullptr)
            {
                return false;
            }
        }
    }
    return true;
}
int StudentWorld::getDisSquare(int x1, int y1, int x2,int y2) 
{
    return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
}
void StudentWorld::pickupOil()
{
    m_tunnelman->pickupOil();
}
bool StudentWorld::squirtArround(int x, int y)
{
    bool rt=false;
    for(int i=0;i<actors.size();i++)
    {
        if(getDisSquare(actors[i]->getX(), actors[i]->getY(), x, y)<=9)
        {
            actors[i]->isAnnoyed();
            rt=true;
        }
    }
    if(getDisSquare(m_tunnelman->getX(), m_tunnelman->getY(), x, y)<=9)
    {
        m_tunnelman->isAnnoyed();
        rt=true;
    }
    return rt;
}

void StudentWorld::getPath(int x, int y, stack<pair<int, int>>& solution)
{
    queue<pair<int, int>> maze;
    //stack<pair<int, int>> solution;
    bool visited[61][61];
    for(int i=0 ;i<61;i++)
    {
        for(int j=0;j<61;j++)
        {
            visited[i][j]=false;
        }
    }
    pair<int,int> parent[61][61];
    pair<int, int> a;
    a.first=x;
    a.second=y;
    visited[x][y]=true;
    maze.push(a);
    while(!maze.empty())
    {
        int x=maze.front().first;
        int y=maze.front().second;
        maze.pop();
        if(x==60 && y==60)
        {
            break;
        }
        if(x>=1 && visited[x-1][y]==false && noEarth(x-1, y))
        {
            visited[x-1][y]=true;
            pair<int, int> temp;
            temp.first=x-1;
            temp.second=y;
            maze.push(temp);
            parent[x-1][y].first=x;
            parent[x-1][y].second=y;
        }
        if(x<60 && visited[x+1][y]==false && noEarth(x+1, y))
        {
            visited[x+1][y]=true;
            pair<int, int> temp;
            temp.first=x+1;
            temp.second=y;
            maze.push(temp);
            parent[x+1][y].first=x;
            parent[x+1][y].second=y;
        }
        if(y<60 && visited[x][y+1]==false && noEarth(x, y+1))
        {
            visited[x][y+1]=true;
            pair<int, int> temp;
            temp.first=x;
            temp.second=y+1;
            maze.push(temp);
            parent[x][y+1].first=x;
            parent[x][y+1].second=y;
        }
        if(y>=1 && visited[x][y-1]==false && noEarth(x, y-1))
        {
            visited[x][y-1]=true;
            pair<int, int> temp;
            temp.first=x;
            temp.second=y-1;
            maze.push(temp);
            parent[x][y-1].first=x;
            parent[x][y-1].second=y;
        }
    }
    int curx=60;
    int cury=60;
    while(curx!=a.first || cury!=a.second)
    {
        int a=parent[curx][cury].first;
        int b=parent[curx][cury].second;
        //cout<<"( "<<curx<<", "<<cury<<" )"<<endl;
        pair<int, int> temp;
        temp.first=curx;
        temp.second=cury;
        solution.push(temp);
        curx=a;
        cury=b;
    }
}
void StudentWorld::getPathtoTunnelman(int x, int y, stack<pair<int, int>>& solution)
{
    queue<pair<int, int>> maze;
    //stack<pair<int, int>> solution;
    bool visited[61][61];
    for(int i=0 ;i<61;i++)
    {
        for(int j=0;j<61;j++)
        {
            visited[i][j]=false;
        }
    }
    pair<int,int> parent[61][61];
    pair<int, int> a;
    a.first=x;
    a.second=y;
    visited[x][y]=true;
    maze.push(a);
    while(!maze.empty())
    {
        int x=maze.front().first;
        int y=maze.front().second;
        maze.pop();
        if(x==m_tunnelman->getX() && y==m_tunnelman->getY())
        {
            break;
        }
        if(x>=1 && visited[x-1][y]==false && noEarth(x-1, y))
        {
            visited[x-1][y]=true;
            pair<int, int> temp;
            temp.first=x-1;
            temp.second=y;
            maze.push(temp);
            parent[x-1][y].first=x;
            parent[x-1][y].second=y;
        }
        if(x<60 && visited[x+1][y]==false && noEarth(x+1, y))
        {
            visited[x+1][y]=true;
            pair<int, int> temp;
            temp.first=x+1;
            temp.second=y;
            maze.push(temp);
            parent[x+1][y].first=x;
            parent[x+1][y].second=y;
        }
        if(y<60 && visited[x][y+1]==false && noEarth(x, y+1))
        {
            visited[x][y+1]=true;
            pair<int, int> temp;
            temp.first=x;
            temp.second=y+1;
            maze.push(temp);
            parent[x][y+1].first=x;
            parent[x][y+1].second=y;
        }
        if(y>=1 && visited[x][y-1]==false && noEarth(x, y-1))
        {
            visited[x][y-1]=true;
            pair<int, int> temp;
            temp.first=x;
            temp.second=y-1;
            maze.push(temp);
            parent[x][y-1].first=x;
            parent[x][y-1].second=y;
        }
    }
    int curx=m_tunnelman->getX();
    int cury=m_tunnelman->getY();
    while(curx!=a.first || cury!=a.second)
    {
        int a=parent[curx][cury].first;
        int b=parent[curx][cury].second;
        //cout<<"( "<<curx<<", "<<cury<<" )"<<endl;
        pair<int, int> temp;
        temp.first=curx;
        temp.second=cury;
        solution.push(temp);
        curx=a;
        cury=b;
    }
}
bool StudentWorld::noWater(int x,int y)
{
    for(int i=x;i<x+4;i++)
    {
        for(int j=y;j<y+4;j++)
        {
            if(water[i][j]==true)
                return false;
        }
    }
    return true;
}
void StudentWorld::fallingRock(int x, int y)
{
    removeEarth2(x, y+1);
    for(int i=0;i<actors.size();i++)
    {
        if(getDisSquare(actors[i]->getX(), actors[i]->getY(), x, y)<=9)
        {
            actors[i]->hitByBoulder();
        }
    }
    if(getDisSquare(m_tunnelman->getX(), m_tunnelman->getY(), x, y)<=9)
        m_tunnelman->hitByBoulder();
}
void StudentWorld::removeEarth(int x,int y)
{
    bool flag=true;
    for(int i=x;i<x+4;i++)
    {
        for(int j=y;j<y+4;j++)
        {
            if(m_earth[i][j]!=nullptr)
            {
                delete m_earth[i][j];
                m_earth[i][j]=nullptr;
                canVisit[i][j]=true;
                flag=false;
            }
        }
        if(flag==false)
            playSound(SOUND_DIG);
    }
    return;
}
void StudentWorld::removeEarth2(int x,int y)
{
    for(int i=x;i<x+4;i++)
    {
        for(int j=y;j<y+4;j++)
        {
            delete m_earth[i][j];
            m_earth[i][j]=nullptr;
            canVisit[i][j]=true;
        }
    }
    return;
}
void StudentWorld::NearbyObject(int x, int y, int rad)
{
    for(int i=0; i<gameObject.size();i++)
    {
        int disX=gameObject[i]->getX()-x;
        int disY=gameObject[i]->getY()-y;
        if(disX*disX+disY*disY<=rad*rad)
        {
            gameObject[i]->setVisible(true);
        }
    }
    return;
}
void StudentWorld::dropGold(int x, int y)
{
    if(x>=0 && x<=60 && y>=0 && y<=60)
        gameObject.push_back(new GoldNugget(x, y,this,true, true));
    return;
}
void StudentWorld::addsquirt(int x,int y, Actor::Direction dir)
{
    if(x>=0 && x<=60 && y>=0 && y<=60)
        gameObject.push_back(new Squirt(x,y,dir,this));
    return;
}
bool StudentWorld::closeToProtester(int x, int y, int n)
{
    for(int i=0; i<actors.size();i++)
    {
        if(getDisSquare(actors[i]->getX(), actors[i]->getY(), x, y)<=(n*n))
        {
            actors[i]->pickupGold();
            return true;
        }
    }
    return false;
}
void StudentWorld::pickupSonar()
{
    m_tunnelman->pickupSonar();
}
void StudentWorld::pickupWater()
{
    m_tunnelman->pickupWater();
}
void StudentWorld::addNewProtester()
{
    int probabilityOfHardcore = fmin(90, getLevel()*10 + 30);
    int temp=rand()%100+1;
    if(temp > probabilityOfHardcore)
        actors.push_back(new RegularProtester(this));
    else
        actors.push_back(new HardcoreProtester(this));
    return;
}
void StudentWorld::upDateText()
{
    std::stringstream text;
    std::string scr="Scr: ";
    std::string lvl="  Lvl: ";
    std::string live="  Lives: ";
    std::string hlth="  Hlth: ";
    std::string wtr="%  Wtr: ";
    std::string gld="  Gld: ";
    std::string sonar="  Sonar: ";
    std::string oilleft="  Oil Left: ";
    text<<scr<<std::setw(6)<<std::setfill('0')<<getScore();
    text<<lvl<<std::setw(2)<<std::setfill(' ')<<getLevel();
    text<<live<<getLives();
    text<<hlth<<std::setw(3)<<std::setfill(' ')<<m_tunnelman->getHealth();
    text<<wtr<<std::setw(2)<<std::setfill(' ')<<m_tunnelman->getWater();
    text<<gld<<std::setw(2)<<std::setfill(' ')<<m_tunnelman->getGold();
    text<<sonar<<std::setw(2)<<std::setfill(' ')<<m_tunnelman->getSonar();
    text<<oilleft<<std::setw(2)<<std::setfill(' ')<<fmin(2 + getLevel(), 21)-m_tunnelman->getOil();
    
    setGameStatText(text.str());
    return;
}
int StudentWorld::init()
{
    for(int i=0;i<64;i++)
    {
        for(int j=0;j<64;j++)
        {
            canVisit[i][j]=true;
            m_earth[i][j]=nullptr;
        }
    }
    for(int i=0;i<61;i++)
    {
        for(int j=0;j<61;j++)
        {
            water[i][j]=false;
        }
    }
    for(int i=0;i<64;i++)
    {
        for(int j=0;j<60;j++)
        {
            if((i<30 || i>33))
            {
                m_earth[i][j]=new Earth(i,j,this);
            }
            else if(j<=3)
            {
                m_earth[i][j]=new Earth(i,j,this);
            }
            else
            {
                m_earth[i][j]=nullptr;
            }
        }
    }
    m_tunnelman=new Tunnelman(this);
    
    int num_Boulder=fmin(getLevel()/ 2 + 2, 9);
    for(int i=0;i<num_Boulder;i++)
    {
        int x=rand()%61; //0-60
        int y=rand()%37+20;  //20-56
        while((x>=27 && x<=33) || x==0 ||x==60||y==56 || noObjectNearby(x, y)==false)
        {
            x=rand()%61;
            y=rand()%37+20;
        }
        gameObject.push_back(new Boulder(x,y,this));
        for(int j=x;j<=x+3;j++)
        {
            for(int k=y;k<=y+3;k++)
            {
                delete m_earth[j][k];
                m_earth[j][k]=nullptr;
                canVisit[j][k]=false;
            }
        }
    }
    
    int num_Gold=fmax(5-getLevel()/ 2, 2);
    for(int i=0;i<num_Gold;i++)
    {
        int x=rand()%61; //0-60
        int y=rand()%57;  //0-56
        while((x>=27 && x<=33) || x==0 ||x==60 ||y==0||y==56 || noObjectNearby(x, y)==false)
        {
            x=rand()%61;
            y=rand()%57;
        }
        gameObject.push_back(new GoldNugget(x,y,this,false,false));
    }
    
    int num_Oil=fmin(2 + getLevel(), 21);
    for(int i=0;i<num_Oil;i++)
    {
        int x=rand()%61; //0-60
        int y=rand()%57;  //0-56
        while(((x>=27 && x<=33))|| x==0 ||x==60 ||y==0||y==56 || noObjectNearby(x, y)==false)
        {
            x=rand()%61;
            y=rand()%57;
        }
        gameObject.push_back(new BarrelOfOil(x,y,this));
    }
    return GWSTATUS_CONTINUE_GAME;
}
bool StudentWorld::closeToTunnelman(int x, int y, int n)
{
    if(getDisSquare(m_tunnelman->getX(), m_tunnelman->getY(), x, y)<=(n*n))
        return true;
    else
        return false;
}

void StudentWorld::pickupGold()
{
    m_tunnelman->pickupGold();
}
bool StudentWorld::intersection(int x, int y, GraphObject::Direction dir)
{
    if(dir==GraphObject::up ||dir==GraphObject::down)
    {
        if(canMove(x, y,GraphObject::left) || canMove(x, y, GraphObject::right))
            return true;
    }
    else if(dir==GraphObject::right ||dir==GraphObject::left)
    {
        if(canMove(x, y,GraphObject::up)|| canMove(x, y, GraphObject::down))
            return true;
    }
    return false;
}
int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    upDateText();
    m_tunnelman->doSomething();
    
    for(int i=0;i<actors.size();i++)
    {
        if(actors[i]->isAlive())
        {
            actors[i]->doSomething();
            if(m_tunnelman->isAlive()==false)
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if(m_tunnelman->getOil()==fmin(2 + getLevel(), 21))
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    for(int i=0;i<gameObject.size();i++)
    {
        if(gameObject[i]->isAlive())
        {
            gameObject[i]->doSomething();
            if(m_tunnelman->isAlive()==false)
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if(m_tunnelman->getOil()==fmin(2 + getLevel(), 21))
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    
    int P = fmin(15, 2 + getLevel() * 1.5);
    int T = fmax(25, 200+getLevel());
    if(firstTick==true)
    {
        addNewProtester();
        firstTick=false;
        countTickP=0;
    }
    else if(countTickP>=T && actors.size()<P)
    {
        addNewProtester();
        countTickP=0;
    }
    countTickP++;
    int G = getLevel() * 25 + 300;
    int temp=rand()%G;
    if(temp==0)
    {
        temp=rand()%5;
        if(temp==0)
        {
            gameObject.push_back(new SonarKit(this));
        }
        else
        {
            int x=rand() % 61;
            int y=rand() % 61;
            while(!noEarth(x,y) || !noWater(x, y))
            {
                x=rand() % 61;
                y=rand() % 61;
            }
            gameObject.push_back(new WaterPool(x, y, this));
            for(int i=x;i<x+4;i++)
            {
                for(int j=y;j<y+4;j++)
                {
                    water[i][j]=true;
                }
            }
        }
    }
    std::vector<Actor*>::iterator it=gameObject.begin();
    while(it!=gameObject.end())
    {
        if(!(*it)->isAlive())
        {
            if(noEarth((*it)->getX(), (*it)->getY()))
            {
                for(int i=(*it)->getX();i<(*it)->getX()+4;i++)
                {
                    for(int j=(*it)->getY();j<(*it)->getY()+4;j++)
                    {
                        water[i][j]=false;
                    }
                }
            }
            //removeEarth2();
            (*it)->setVisible(false);
            //std::vector<Actor*>::iterator temp=it+1;
            it=gameObject.erase(it);
            //it=temp;
        }
        else
            it++;
    }
    it=actors.begin();
    while(it!=actors.end())
    {
        if(!(*it)->isAlive())
        {
            //removeEarth2((*it)->getX(), (*it)->getY());
            (*it)->setVisible(false);
            //std::vector<Actor*>::iterator temp=it+1;
            it=actors.erase(it);
            //it=temp;
        }
        else
            it++;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (int i=0; i<64; i++)
    {
        for (int j=0; j<64; j++)
        {
            delete m_earth[i][j];
            m_earth[i][j]=nullptr;
        }
    }
    std::vector<Actor*>::iterator it=gameObject.begin();
    while(it!=gameObject.end())
    {
        //std::vector<Actor*>::iterator temp=it+1;
        (*it)->setVisible(false);
        it=gameObject.erase(it);
        //it=temp;
    }
    it=actors.begin();
    while(it!=actors.end())
    {
        //std::vector<Actor*>::iterator temp=it+1;
        (*it)->setVisible(false);
        it=actors.erase(it);
        //it=temp;
    }
    delete m_tunnelman;
}
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

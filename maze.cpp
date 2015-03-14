
#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include "maze.h"

using namespace std;

int Maze::highestLvl = 1;

Maze::Maze(int rows, int cols, float range, int level):endPos(-1, -1)
{
    if(!(rows & cols & 1)) exit(1); //rows and cols must be odd number!
    this->rows = rows;
    this->cols = cols;
    this->range = range;
    this->level = level;

    if(level >highestLvl)
        highestLvl = level;

    this->index = index;
    map = new Node[rows*cols];

    at(0,0).id = ROAD;
    at(0,1).id = ROAD;
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            at(i,j).root = at(i,j).index = i*cols+j;
    createMaze();
}

Maze::~Maze(){
    delete[] map;
}

Node& Maze::getRoot(Node& n)const
{
    if(n.root != n.index)
        return getRoot(map[n.root]);
    else
        return n;
}

void Maze::createMaze()
{
    list<Edge> edges;
    for(int i=1; i<=rows-2; i+=2)
    {
        for(int j=1; j<cols; j+=2)
        {
               Position center(i,j);
               Position down = center.down(2);
               Position right = center.right(2);
               if(down.isInRange(1, rows-2, 1, cols-2))
                   edges.push_back(Edge(center,down));
               if(right.isInRange(1, rows-2, 1, cols-2))
                   edges.push_back(Edge(center,right));
        }
    }

    list<Edge>::iterator it = edges.begin();
    for(int i=0; i<(rows-1)*(cols-1)/4 - 1; i++)    //Randomly generated N-1 edges that are not ring, construct a tree
    {
        while(true)     //Randomly find a legal edge
        {
            int index = rand()%edges.size();
            for(it=edges.begin(); index>0; index--)     //set 'it' to the edge
                it++;
            Node& pr = getRoot(at(it->p));
            Node& qr = getRoot(at(it->q));
            if(pr.root == qr.root)
            {
                edges.erase(it);
                continue;
            }
            at(it->p).id = ROAD;    //sign this edge(three point) as road
            at(it->q).id = ROAD;
            at(it->p.getMiddle(it->q)).id = ROAD;
            at(it->p).count++;
            at(it->q).count++;
            pr.root = qr.root;
            break;
        }
    }
}

void Maze::initialize(int rows, int cols, float range, int level)
{
    if(map != NULL)
    {
        delete[] map;
        map = NULL;
    }
    portals.clear();
    endPos.x = -1;
    endPos.y = -1;
    if(!(rows & cols & 1)) exit(1); //rows and cols must be odd number!
    this->rows = rows;
    this->cols = cols;
    this->range = range;
    this->level = level;

    if(level > highestLvl)
        highestLvl = level;
    this->index = index;
    map = new Node[rows*cols];
    at(0,0).id = ROAD;
    at(0,1).id = ROAD;
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            at(i,j).root = at(i,j).index = i*cols+j;    //initialise every node's root and index of the maze as it's array index
    createMaze();
}

void Maze::setPortalTo(Maze &mt, int count)
{
    for(int i=0; i<count; i++)
    {
        Position here = randomValidPos(1);
        while(at(here).id != ROAD)
            here = randomValidPos(1);
        at(here).id = PORTAL;
        Position there = randomValidPos(1);
        while(mt.at(there).id != ROAD)
            there = mt.randomValidPos(1);
        mt.at(there).id = PORTAL;
        Portal portal_here(here, &mt, there);
        Portal portal_there(there, this, here);
        portals.push_back(portal_here);
        mt.portals.push_back(portal_there);
    }
}

const Portal& Maze::findPortal(int x, int y) const
{
    for(int i=0; i<portals.size(); i++)
        if(portals[i].posHere.x == x && portals[i].posHere.y == y)
            return portals[i];
}

const Portal& Maze::findPortal(const Maze& m) const
{
    for(int i=0; i<portals.size(); i++)
        if(portals[i].dest == &m)
            return portals[i];
}

Position Maze::randomValidPos(int minCount) const
{
    vector<Position> validPos;
    for(int i=1; i<rows-2; i+=2)
        for(int j=1; j<cols-2; j+=2)
            if(at(i,j).count >= minCount)
                validPos.push_back(Position(i,j));
    return validPos[rand()%validPos.size()];
}

Position Maze::setEnd()
{
    int i = rand()%3;
    Position p(rows-1, cols -2);
    switch(i)
    {
    case 0: ;break;
    case 1: p = Position(0, cols-2);break;
    case 2: p = Position(rows-1, 1);break;
    }
    at(p).id = END;
    endPos = p;
    return p;
}

Position Maze::setEnd(const Maze& m)
{
    Portal p = findPortal(m);
    endPos = p.posHere;
    return endPos;
}

Position Maze::setStart()
{
    Position p(1,1);
    at(p).id = START;
    return p;
}

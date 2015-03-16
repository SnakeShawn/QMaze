#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include "portal.h"
#include "position.h"

enum role{OUT,WALL,ROAD,PORTAL,TRAP,START,END};

////////the Node of the Maze
class Node{
public:
    int id;     //type of the node, i.e. role{}
    int index;
    int root;
    int count;
    bool inPath;
    bool isPath(){return inPath;}
    Node():id(WALL), root(0), index(0), count(0), inPath(false){}   //initialize all node as WALL
};

////////the Class of the Maze
class Maze{
public:
    float range;    //field of vision
    int level;
    static int highestLvl;
private:
    Node* map;
    int rows;
    int cols;
    int index;

    Position endPos;

    std::vector<Portal> portals;
    std::vector<Portal> traps;

    class Edge{
        friend class Maze;
    private:
        Position p;
        Position q;
        Edge(Position a, Position b):p(a),q(b){}
    };

public:
    Maze():map(NULL), endPos(-1,-1){}
    Maze(int rows, int cols, float range, int level);
    ~Maze();
    void initialize(int rows, int cols, float range, int level);
    int operator()(int x, int y)const{  //overload operator '()', to use () access the id of Node
        if(x<0||x>rows-1||y<0||y>cols-1)
            return OUT;                 //Out of the Maze
        return map[x*cols+y].id;
    }
    Node& at(int x, int y)const{return map[x*cols+y];}
    void setPortalTo(Maze& m, int count);
    void setTrapTo(Maze& m, int count);
    void deleteTrap(int x, int y);
    const Portal& findPortal(int x, int y)const;
    const Portal& findPortal(const Maze&)const;
    bool findPathTo(Position origin, Position dest);
    Position randomValidPos(int minCount)const;
    Position randomNextPos(Position next,Position curr)const;
    Position setEnd();
    Position setEnd(const Maze&);
    Position setStart();
    int getRow()const{return rows;}
    int getCol()const{return cols;}
    Position getEnd()const{return endPos;}
private:
    Node& at(const Position& p)const{return map[p.x*cols+p.y];}
    Node& getRoot(Node&)const;
    bool isNextPosInPath(Position last, Position curr, Position dest);
    void createMaze();

};

#endif // MAZE


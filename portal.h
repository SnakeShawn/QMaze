#ifndef PORTAL_H
#define PORTAL_H

#include "maze.h"
#include "position.h"

class Position;
class Maze;

class Portal{
public:
    Position posHere;
    Maze* dest;
    Position posThere;
    Portal(Position here, Maze* m, Position there):posHere(here), dest(m), posThere(there){}
};

#endif // PORTAL


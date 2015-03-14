#ifndef MAZEGROUP
#define MAZEGROUP

#include "maze.h"

class MazeGroup{
    const static int rows = 27;
    const static int cols = 45;
public:
    MazeGroup();
    void reinitialize();
    Maze* getStartMaze(int level);
private:
    void setLevel_1();
    void setLevel_2();
    void setLevel_3();
    void setLevel_4();
    Maze level_1[1];
    Maze level_2[3];
    Maze level_3[5];
    Maze level_4[7];
};

#endif // MAZEGROUP


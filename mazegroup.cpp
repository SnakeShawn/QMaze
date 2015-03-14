
#include <cstdlib>
#include <ctime>
#include "mazegroup.h"

MazeGroup::MazeGroup()
{
    reinitialize();
}

void MazeGroup::reinitialize()
{
    srand((unsigned)time(NULL));
    for(int i=0; i<1; i++)
        level_1[i].initialize(rows, cols, 100, 1);
    for(int i=0; i<3; i++)
        level_2[i].initialize(rows, cols, 10, 2);
    for(int i=0; i<5; i++)
        level_3[i].initialize(rows, cols, 7, 3);
    for(int i=0; i<7; i++)
        level_4[i].initialize(rows, cols, 5, 4);
    setLevel_1();
    setLevel_2();
    setLevel_3();
    setLevel_4();
}

Maze* MazeGroup::getStartMaze(int level)
{
    switch(level)
    {
    case 1:return &level_1[0];
    case 2:return &level_2[0];
    case 3:return &level_3[0];
    case 4:return &level_4[0];
    default:return NULL;
    }
}

void MazeGroup::setLevel_1()
{
    level_1[0].setEnd();
}

void MazeGroup::setLevel_2()
{
    level_2[0].setPortalTo(level_2[1],3);
    level_2[0].setPortalTo(level_2[2],1);
    level_2[1].setPortalTo(level_2[2],1);
    level_2[0].setEnd(level_2[2]);
    level_2[1].setEnd(level_2[2]);
    level_2[2].setEnd();
}

void MazeGroup::setLevel_3()
{
    level_3[0].setPortalTo(level_3[1],2);
    level_3[0].setPortalTo(level_3[2],2);
    level_3[1].setPortalTo(level_3[2],1);
    level_3[1].setPortalTo(level_3[3],2);
    level_3[2].setPortalTo(level_3[4],1);
//    for(int i = 0;i<5;i++)
//    {
//        level_3[i].setTrapTo(level_2[rand()%3],3);
//    }
    level_3[0].setEnd(level_3[2]);
    level_3[1].setEnd(level_3[2]);
    level_3[2].setEnd(level_3[4]);
    level_3[3].setEnd(level_3[1]);
    level_3[4].setEnd();
}

void MazeGroup::setLevel_4()
{
    level_4[0].setPortalTo(level_4[1],4);
    level_4[0].setPortalTo(level_4[2],1);
    level_4[1].setPortalTo(level_4[3],2);
    level_4[1].setPortalTo(level_4[4],2);
    level_4[2].setPortalTo(level_4[5],3);
    level_4[2].setPortalTo(level_4[6],1);
    level_4[5].setPortalTo(level_4[6],1);
//    for(int i = 0;i<5;i++)
//    {
//        level_4[i].setTrapTo(level_3[rand()%5],3);
//    }
//    level_4[6].setTrapTo(level_3[rand()%5],3);
    level_4[0].setEnd(level_4[2]);
    level_4[1].setEnd(level_4[0]);
    level_4[2].setEnd(level_4[6]);
    level_4[3].setEnd(level_4[1]);
    level_4[4].setEnd(level_4[1]);
    level_4[5].setEnd(level_4[6]);
    level_4[6].setEnd();
}

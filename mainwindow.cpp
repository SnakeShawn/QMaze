#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <phonon/MediaObject>
#include <phonon/MediaSource>
#include <phonon/phonon>
using namespace Phonon;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    wallImg(":/pic/resources/pic/wall.jpg"),
    roadImg(":/pic/resources/pic/road.jpg"),
    pathImg(":/pic/resources/pic/path.jpg"),
    endImg(":/pic/resources/pic/end.jpg"),
    portalImg(":/pic/resources/pic/portal.jpg"),
    manImg(":/pic/resources/pic/man.jpg"),
    currMusic(":/resources/music/background.wma")
{
    ui->setupUi(this);
    setWindowTitle("Tomb Adventure盗墓笔记");
//    setWindowTitle("Tomb Adventure");
    setFixedSize(1000,620);

    bgPlayer = createPlayer(MusicCategory,currMusic);
    connect(bgPlayer,SIGNAL(finished()),SLOT(onMusicFinished()));
    bgPlayer->play();

    noWall = false;
    showTrap = true;
    viewOpen = true;
    manCurrX = 0;
    manCurrY = 0;

    currMaze = mazeGroup.getStartMaze(3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reinitialize()
{
    mazeGroup.reinitialize();
    currMaze = mazeGroup.getStartMaze(1);
    manCurrX = 0;
    manCurrY = 0;
    update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter q(this);
    q.setPen(q.background().color());
    for(int i=0;i<(*currMaze).getRow();i++)
        for(int j=0;j<(*currMaze).getCol();j++)
        {
            if( !viewOpen && ((i-manCurrY)*(i-manCurrY)+(j-manCurrX)*(j-manCurrX)>(*currMaze).range*(*currMaze).range)
                &&((i-manCurrY+1)*(i-manCurrY+1)+(j-manCurrX)*(j-manCurrX)>(*currMaze).range*(*currMaze).range)
                &&((i-manCurrY)*(i-manCurrY)+(j-manCurrX+1)*(j-manCurrX+1)>(*currMaze).range*(*currMaze).range)
                &&((i-manCurrY+1)*(i-manCurrY+1)+(j-manCurrX+1)*(j-manCurrX+1)>(*currMaze).range*(*currMaze).range)\
                )
                continue;
            switch((*currMaze)(i,j))
            {
                case WALL:{
                    q.drawImage(MazePosX+j*imgWidth,MazePosY+i*imgHeight,wallImg);
                    break;
                }
                case ROAD:{
                    q.drawImage(MazePosX+j*imgWidth,MazePosY+i*imgHeight,roadImg);
                    break;
                }
                case PORTAL:{
                    q.drawImage(MazePosX+j*imgWidth,MazePosY+i*imgHeight,portalImg);
                    break;
                }
                case TRAP:{
                    if(showTrap)
                        q.drawImage(MazePosX+j*imgWidth,MazePosY+i*imgHeight,trapImg);
                    else
                         q.drawImage(MazePosX+j*imgWidth,MazePosY+i*imgHeight,roadImg);
                    break;
                }
                case END:{
                    q.drawImage(MazePosX+j*imgWidth,MazePosY+i*imgHeight,endImg);
                    break;
                }
            }

            if(!viewOpen)
            {
            for(int y =MazePosY+i*imgHeight;y<MazePosX+(i+1)*imgHeight;y++)
                for(int x =MazePosX+j*imgWidth;x<MazePosX+(j+1)*imgHeight;x++)
                    if((y-manCurrY*imgHeight-MazePosY)*(y-manCurrY*imgHeight-MazePosY)
                        +(x-manCurrX*imgWidth-MazePosX)*(x-manCurrX*imgWidth-MazePosY)
                            >(*currMaze).range*(*currMaze).range*imgWidth*imgHeight)
                        q.drawPoint(x,y);
            }
        }

    q.drawImage(MazePosX+imgWidth*manCurrX,MazePosX+imgHeight*manCurrY,manImg);
}

void MainWindow::keyPressEvent(QKeyEvent *k)
{
    if(k->key() == Qt::Key_W)
    {
        if((noWall || (*currMaze)(manCurrY-1, manCurrX) != WALL) && (*currMaze)(manCurrY-1, manCurrX) != OUT){
            manCurrY --;
            onMove();
        }
    }
    if(k->key() == Qt::Key_S)
    {
        if((noWall || (*currMaze)(manCurrY+1, manCurrX) != WALL) && (*currMaze)(manCurrY+1, manCurrX) != OUT){
            manCurrY ++;
            onMove();
        }
    }
    if(k->key() == Qt::Key_A)
    {
        if((noWall || (*currMaze)(manCurrY, manCurrX-1) != WALL) && (*currMaze)(manCurrY, manCurrX-1) != OUT){
            manCurrX --;
            onMove();
        }
    }
    if(k->key() == Qt::Key_D)
    {
        if((noWall || (*currMaze)(manCurrY, manCurrX+1) != WALL) && (*currMaze)(manCurrY, manCurrX+1) != OUT){
            manCurrX ++;
            onMove();
        }
    }
    if(k->key() == Qt::Key_Space)
    {
    }
    update();
}

void MainWindow::onMove()
{
    switch ((*currMaze)(manCurrY, manCurrX)) {
    case TRAP:
        break;
    case END:{
        if(currMaze->level == Maze::highestLvl)
        {
        }
        else
        {
            QMessageBox endMsg;
            endMsg.setText("眼前的甬道通往魔宫更黑暗的深处，吴邪踌躇了一会儿，便走进了黑暗之中。。。。");
            endMsg.exec();
        }
        break;
    }
    default:
        break;
    }
}

void MainWindow::onMusicFinished()
{
    bgPlayer->setCurrentSource(currMusic);
    bgPlayer->play();
}

void MainWindow::on_newGame_triggered()
{
    reinitialize();
}

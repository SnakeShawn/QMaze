#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameinfo.h"
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
    trapImg(":/pic/resources/pic/trap.jpg"),
    endImg(":/pic/resources/pic/end.jpg"),
    portalImg(":/pic/resources/pic/portal.jpg"),
    manImg(":/pic/resources/pic/man.jpg"),
    currMusic(":/resources/music/background.wma")
{
    ui->setupUi(this);

    setWindowTitle("Tomb Adventure");
    setFixedSize(1000,620);

    bgPlayer = createPlayer(MusicCategory,currMusic);
    connect(bgPlayer,SIGNAL(finished()),SLOT(onMusicFinished()));
    bgPlayer->play();

    noWall = false;
    showTrap = true;
    showPath = false;
    viewOpen = true;
    manCurrX = 0;
    manCurrY = 0;

    currMaze = mazeGroup.getStartMaze(4);
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

            if(showPath)
                if(currMaze->at(i,j).isPath())
                    q.drawImage(MazePosX+j*imgWidth,MazePosY+i*imgHeight,pathImg);

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
        if((*currMaze)(manCurrY,manCurrX) == PORTAL)
        {
            Portal ptl = (*currMaze).findPortal(manCurrY,manCurrX);
            if(currMaze != ptl.dest)
            {
                currMaze = ptl.dest;
                //reset the zombie
            }
            else
            {
                currMaze = ptl.dest;
            }
            manCurrY = ptl.posThere.x;
            manCurrX = ptl.posThere.y;
        }
        showPath = false;
        ui->showPath->setChecked(false);
    }
    update();
}

void MainWindow::onMove()
{
//    if(meetZombie(Position(manCurrY, manCurrX)))
//        gameOver();
    switch ((*currMaze)(manCurrY, manCurrX)) {
    case TRAP:
    {
        Portal trap = currMaze->findPortal(manCurrY, manCurrX);
        currMaze->deleteTrap(manCurrY, manCurrX);
        QMessageBox trapMsg;
        trapMsg.setText("Trap!");
        trapMsg.exec();
        if (currMaze != trap.dest)
        {
            currMaze = trap.dest;
//            resetZombie();
        }
        else
        {
            currMaze = trap.dest;
        }
        manCurrX = trap.posThere.y;
        manCurrY = trap.posThere.x;
//    if(meetZombie(Position(manCurrY, manCurrX)))
//        gameOver();
        showPath = false;
        ui->showPath->setChecked(false);
        update();
        break;
    }
    case END:{
        if(currMaze->level == Maze::highestLvl)
        {
            QMessageBox endMsg;
            endMsg.setText("Suddenly。。。。");
            endMsg.exec();
            on_newGame_triggered();
            break;
        }
        else
        {
            QMessageBox endMsg;
            endMsg.setText("Next Level。。。。");
            endMsg.exec();
            on_nextLevel_triggered();
            showPath = false;
            ui->showPath->setChecked(false);
            break;
        }
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

void MainWindow::on_nextLevel_triggered()
{
    if(currMaze->level < Maze::highestLvl)
    {
        currMaze = mazeGroup.getStartMaze(currMaze->level + 1);
        manCurrX = 0;
        manCurrY = 0;
        update();
    }
}

void MainWindow::on_lastLevel_triggered()
{
    if(currMaze->level > 1)
    {
        currMaze = mazeGroup.getStartMaze(currMaze->level - 1);
        manCurrX = 0;
        manCurrY = 0;
        update();
    }
}

void MainWindow::on_Music_changed()
{
    if(ui->Music->isChecked())
    {
        bgPlayer->play();
    }
    else
    {
        bgPlayer->pause();
    }
}

void MainWindow::on_noWall_changed()
{
    if(ui->noWall->isChecked())
        noWall = true;
    else
        noWall = false;
}

void MainWindow::on_viewOpen_changed()
{
    if(ui->viewOpen->isChecked())
    {
        viewOpen = true;
    }
    else
    {
        viewOpen = false;
    }
    update();
}

void MainWindow::on_showPath_changed()
{
    if(ui->showPath->isChecked())
    {
        Position endPos = currMaze->getEnd();
        if(endPos == Position(-1, -1))
            return ;
        currMaze->findPathTo(Position(manCurrY, manCurrX), endPos);
        showPath = true;
    }
    else
    {
        showPath = false;
    }
    update();
}

void MainWindow::on_showTrap_changed()
{
    if(ui->showTrap->isChecked())
    {
        showTrap = true;
    }
    else
    {
        showTrap = false;
    }
    update();
}

void MainWindow::on_About_triggered()
{
    QMessageBox about;
    about.setText("Maze v.1.1");
    about.exec();
}

void MainWindow::on_Help_triggered()
{
    GameInfo info;
    info.exec();
}

void MainWindow::resetOptions()
{}

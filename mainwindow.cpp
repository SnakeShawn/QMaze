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
    preciousImg(":/pic/resources/pic/precious.jpg"),
    portalImg(":/pic/resources/pic/portal.jpg"),
    manImg(":/pic/resources/pic/man.jpg"),
    zombieImg(":/pic/resources/pic/zombie.jpg"),
    currMusic(":/resources/music/background.wma")
{
    ui->setupUi(this);

//    setWindowTitle("Tomb Adventure");
    setWindowTitle("盗墓笔记之七星鲁王宫");
    setFixedSize(1000,620);

    bgPlayer = createPlayer(MusicCategory,currMusic);
    connect(bgPlayer,SIGNAL(finished()),SLOT(onMusicFinished()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(moveZombie()));
    timer->start(1000); // 1 second

    bgPlayer->play();
    currMaze = mazeGroup.getStartMaze(1);
    resetZombie();

    noWall = false;
    showTrap = false;
    showPath = false;
    viewOpen = false;
    manCurrX = 0;
    manCurrY = 0;


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
                    if(currMaze->level == Maze::highestLvl)
                    {
                        q.drawImage(MazePosX+j*imgWidth,MazePosY+i*imgHeight,preciousImg);
                    }
                    else
                    {
                        q.drawImage(MazePosX+j*imgWidth,MazePosY+i*imgHeight,endImg);
                    }
                    break;
                }
            }

            if(showPath)
                if(currMaze->at(i,j).isPath())
                    q.drawImage(MazePosX+j*imgWidth,MazePosY+i*imgHeight,pathImg);

            if(meetZombie(Position(i,j)))
                q.drawImage(MazePosX+j*imgWidth,MazePosY+i*imgHeight,zombieImg);
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
                resetZombie();
            }
            else
            {
                currMaze = ptl.dest;
            }
            manCurrY = ptl.posThere.x;
            manCurrX = ptl.posThere.y;
            resetOptions();
        }
        if(meetZombie(Position(manCurrY, manCurrX)))
            gameOver();
        showPath = false;
        ui->showPath->setChecked(false);
    }
    update();
}

void MainWindow::onMove()
{
    if(meetZombie(Position(manCurrY, manCurrX)))
        gameOver();
    switch ((*currMaze)(manCurrY, manCurrX)) {
    case TRAP:
    {
        Portal trap = currMaze->findPortal(manCurrY, manCurrX);
        currMaze->deleteTrap(manCurrY, manCurrX);
        QMessageBox trapMsg;
        trapMsg.setText("你脚下一滑掉入了一口棺材，然后。。。");
        trapMsg.exec();
        if (currMaze != trap.dest)
        {
            currMaze = trap.dest;
            resetZombie();
        }
        else
        {
            currMaze = trap.dest;
        }
        manCurrX = trap.posThere.y;
        manCurrY = trap.posThere.x;
        if(meetZombie(Position(manCurrY, manCurrX)))
            gameOver();
        resetOptions();
        update();
        break;
    }
    case END:{
        if(currMaze->level == Maze::highestLvl)
        {
            QMessageBox endMsg;
            endMsg.setText("啊，宝藏就在眼前了，这时候突然。。。。");
            endMsg.exec();
            on_newGame_triggered();
            break;
        }
        else
        {
            QMessageBox endMsg;
            endMsg.setText("眼前的甬道通往魔宫的更深处，吴邪踌躇了一会儿，便走进了黑暗之中。。。。");
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

void MainWindow::moveZombie()
{
    for(int i=0; i<zombieCount; i++)
    {
        Position p = currMaze->randomNextPos(zombiesJustNow[i],zombies[i]);
        zombiesJustNow[i] = zombies[i];
        zombies[i] = p;
    }
    update();
    if(meetZombie(Position(manCurrY,manCurrX)))
        gameOver();
}

void MainWindow::resetZombie()
{
    for (int i=0; i<zombieCount; i++)
        zombies[i] = currMaze->randomValidPos(1);
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
        resetOptions();
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
        resetOptions();
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

void MainWindow::gameOver()
{
    QMessageBox msg;
    msg.setText("看来是遇到了千年血粽子，只好退出墓穴重整旗鼓了");
    msg.exec();
    on_newGame_triggered();
}

void MainWindow::resetOptions()
{
    showPath = false;
    ui->showPath->setChecked(false);
    noWall = false;
    ui->noWall->setChecked(false);
    showTrap = false;
    ui->showTrap->setChecked(false);
    viewOpen = false;
    ui->viewOpen->setChecked(false);
}

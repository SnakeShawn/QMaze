#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <phonon/MediaObject>
#include <phonon/MediaSource>
#include "maze.h"
#include "mazegroup.h"

namespace Ui {
class MainWindow;
}

class ZombieThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void onMove();
    void resetOptions();
    void reinitialize();

private slots:
    void moveZombie();
    void onMusicFinished();

    void on_newGame_triggered();

    void on_nextLevel_triggered();

    void on_lastLevel_triggered();

    void on_Music_changed();

    void on_noWall_changed();

    void on_viewOpen_changed();

    void on_showPath_changed();

    void on_showTrap_changed();

    void on_About_triggered();

    void on_Help_triggered();

private:
    Ui::MainWindow *ui;
    const static int MazePosX = 50;
    const static int MazePosY = 50;
    const static int imgWidth = 20;
    const static int imgHeight= 20;
    const static int zombieCount = 4;
    MazeGroup mazeGroup;
    Maze* currMaze;
    ZombieThread* zombie;
    Phonon::MediaObject* bgPlayer;
    Phonon::MediaSource currMusic;
    QImage wallImg, roadImg, pathImg, endImg, portalImg, trapImg, manImg, zombieImg, preciousImg;
    int manCurrX;
    int manCurrY;
    bool showTrap;
    bool showPath;
    bool noWall;
    bool viewOpen;

    Position zombies[zombieCount];
    Position zombiesJustNow[zombieCount];
    bool meetZombie(Position p)
    {
        if(currMaze->level < 4)
            return false;
        for(int i=0; i<zombieCount; i++)
            if(p == zombies[i])
                return true;
        return false;
    }
    void resetZombie();
    void gameOver();
};


#endif // MAINWINDOW_H

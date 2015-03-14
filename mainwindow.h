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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void onMove();
    void reinitialize();
private slots:
    void onMusicFinished();
    void on_newGame_triggered();
private:
    Ui::MainWindow *ui;
    const static int MazePosX = 50;
    const static int MazePosY = 50;
    const static int imgWidth = 20;
    const static int imgHeight= 20;
    MazeGroup mazeGroup;
    Maze* currMaze;
    Phonon::MediaObject* bgPlayer;
    Phonon::MediaSource currMusic;
    QImage wallImg, roadImg, pathImg, endImg, portalImg, trapImg, manImg, zombieImg;
    int manCurrX;
    int manCurrY;
    bool showTrap;
//    bool showPath;
    bool noWall;
    bool viewOpen;
};

#endif // MAINWINDOW_H

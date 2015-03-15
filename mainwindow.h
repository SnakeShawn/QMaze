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

    void on_nextLevel_triggered();

    void on_lastLevel_triggered();

    void on_Music_changed();

    void on_noWall_changed();

    void on_viewOpen_changed();

    void on_showPath_changed();

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
    bool showPath;
    bool noWall;
    bool viewOpen;
};

#endif // MAINWINDOW_H

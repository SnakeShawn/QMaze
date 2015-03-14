#-------------------------------------------------
#
# Project created by QtCreator 2015-03-12T12:15:29
#
#-------------------------------------------------

QT       += core gui
QT  +=phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QMaze
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    maze.cpp \
    mazegroup.cpp

HEADERS  += mainwindow.h \
    maze.h \
    position.h \
    mazegroup.h \
    portal.h

FORMS    += mainwindow.ui

RESOURCES += \
    pic.qrc \
    music.qrc

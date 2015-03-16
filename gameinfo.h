#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <QDialog>

namespace Ui {
class GameInfo;
}

class GameInfo : public QDialog
{
    Q_OBJECT

public:
    explicit GameInfo(QWidget *parent = 0);
    ~GameInfo();

private:
    Ui::GameInfo *ui;
};

#endif // GAMEINFO_H

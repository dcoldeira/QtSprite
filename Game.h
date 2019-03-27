#ifndef PLAYER_H
#define PLAYER_H
#include "KeyEvent.h"



class Game: public KeyEvent
{
public:
    Game();
    KeyEvent *capy;
    QGraphicsScene * scene;

};

#endif // PLAYER_H

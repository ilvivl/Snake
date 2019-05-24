#pragma once

#include "game.h"
#include "keypressable.h"


class Control
{
public:
    Control(Snake* s, Game* g);

protected:
    Snake* model;
    Game*  game;
};


class CHuman : public Control, public Keypressable
{
public:
    CHuman(Snake* s, Game* g);
    void onkey(const int key);
};

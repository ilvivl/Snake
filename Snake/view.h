#pragma once

#include <functional>
#include <utility>

#include "game.h"
#include "keypressable.h"

class View
{
    using timer_fn = std::function<void(void)>;
public:
    virtual void draw() = 0;
    virtual void run() = 0;
    virtual void snakepainter(Coord c, Dir d) = 0;
    virtual void rabbitpainter(Coord c) = 0;
    virtual ~View() = 0; 

    void set_onkey(Keypressable* k);
    void set_model(Game* g);
    void set_ontimer(int timeout, timer_fn fn);

    static View* get();

protected://только он и подклассы
    std::pair<int, timer_fn> timer;
    Keypressable* onkey_delegate;
    Game*         game;

private:
    static View* inst;
};

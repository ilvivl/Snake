// #include "view.h"
#include "tui.h"
#include "keypressable.h"


View::~View() {}


View* View::inst = nullptr;

View* View::get()
{
    if (!inst)
        inst = new Tui;

    return inst;
}


void View::set_onkey(Keypressable* k)
{
    onkey_delegate = k;
}


void View::set_model(Game* g)
{
    game = g;
}


void View::set_ontimer(int timeout, timer_fn fn)
{
    timer = {timeout, fn};
}

// #include "view.h"
#include "tui.h"
#include "control.h"
#include "game.h"

int main()
{
    View* v = View::get();

    Snake s;
    Game g;
    CHuman h(&s, &g);
    g.add(&s);

    v->draw();
    v->run();
    delete v;

    return 0;
}

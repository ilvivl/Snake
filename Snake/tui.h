#pragma once

#include <termios.h>//распознование нажатия клавиш в потоковом режиме

#include "view.h"

class Tui : public View
{
public:
    Tui();
    void draw();
    void run();
    void snakepainter(Coord c, Dir d);
    void rabbitpainter(Coord c);
    ~Tui(); //деструктор

private:
    int col;
    int row;
    struct termios old_at;

    void get_winsize();
    void gotoxy(const int x, const int y);
    void draw_hline(const int x, const int y, const int size);
    void draw_vline(const int x, const int y, const int size);
    void clear_screen();
};


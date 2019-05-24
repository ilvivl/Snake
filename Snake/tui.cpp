#include <sys/ioctl.h> //работа с терминалом
#include <termios.h> //распознование нажатия клавиш в потоковом режиме
#include <unistd.h>
#include <iostream>
#include <signal.h>//работа с cигналами
#include <functional>
#include <poll.h> //функция запроса
using namespace std::placeholders; //для _1, _2 вместо аргумента берём, какой, аргумент, переданный 

#include "tui.h"


static void win_handler(int sig)
{
    View* v = View::get();
    v->draw();
}


Tui::Tui()
{
    get_winsize();

    struct sigaction sa; //для изменения действий процесса при получении сигнала этим занимается sigaction()
    sa.sa_handler = win_handler; //на что заменяем, указатель на функцию обработки сигнала
    sa.sa_flags = SA_RESTART; // флаг, позволяющий работать системным вызовам во время обработки сигнала
    sigaction(SIGWINCH, &sa, NULL); //SIGWINCH изменение окна

    struct termios at;
    tcgetattr(0, &at);//получает параметры терминала и сохраняет их в 2 аргумент  
    old_at = at;
    cfmakeraw(&at); //превращает дааные терминала в строку
    tcsetattr(0, TCSAFLUSH, &at); //устанавливает параметры терминала
}


void Tui::get_winsize()
{
    struct winsize ws;
    ioctl(1, TIOCGWINSZ, &ws);//узнаёт максимальный размер окна

    this->col = ws.ws_col;
    this->row = ws.ws_row;
}


void Tui::gotoxy(const int x, const int y)
{
    printf("\e[%d;%dH", y, x); //уход за верхнюю границу
}


void Tui::draw_hline(const int x, const int y, const int size)
{
    for (int i = 0; i < size; ++i)
    {
        gotoxy(x + i, y);
        if (!i || i == size - 1)
            printf("+");
        else
            printf("-");
    }
}

//сделать по-другому
void Tui::draw_vline(const int x, const int y, const int size)
{
    for (int i = 0; i < size; ++i)
    {
        gotoxy(x, y + i);
        if(!i)
            printf("+");
        else
            printf("|");
    }
}


void Tui::clear_screen()
{
    printf("\e[H\e[J");
}


void Tui::draw()
{
    clear_screen();
    get_winsize();
    gotoxy(col/2, 1);
    printf("Snake");
    draw_hline(2, 2, col - 2);
    draw_vline(2, 2, row - 3);
    draw_hline(2, row - 1, col - 2);
    draw_vline(col - 1, 2, row - 3);
    gotoxy(col/2 - 10, row);
    printf("Vinogradiov Ilya, 2019");

    SnakePainter  sp = std::bind(&View::snakepainter, this, _1, _2);
    RabbitPainter rp = std::bind(&View::rabbitpainter, this, _1);
    game->visit(sp, rp);

    gotoxy(0, 0);
    fflush(stdout); //сброс буфера
}


void Tui::run()
{
    if (!onkey_delegate)
        return;

    int time_delay = 0;
    for(;;)
    {
        struct timespec start_tp;
        clock_gettime(CLOCK_REALTIME, &start_tp);//записывает время

        struct pollfd fds;
        fds.fd = STDIN_FILENO;//лежит прочитанный символ
        fds.events = POLLIN;//чтение
        int res = poll(&fds, 1, timer.first - time_delay);//ожидает событие нажатия
        if (res > 0)
        {
            char sym;
            read(STDIN_FILENO, &sym, 1); //читаем символ
            if (sym == 'q')
                return;

            onkey_delegate->onkey(sym);

            struct timespec end_tp;
            clock_gettime(CLOCK_REALTIME, &end_tp);
            time_delay = ((end_tp.tv_sec - start_tp.tv_sec) * 1000 + (end_tp.tv_nsec - start_tp.tv_nsec)) / 1000000;//секунды и милисекунды
        }
        else if (res == 0)
        {
            timer.second();
            draw();
            time_delay = 0;
        }
        else
        {
            //error exception
        }
    }
}


void Tui::snakepainter(Coord c, Dir d)
{
    gotoxy(c.first, c.second);
    putchar("^v<>o"[d]);
}

void Tui::rabbitpainter(Coord c)
{
    gotoxy(c.first, c.second);
    putchar('$');
}


Tui::~Tui()
{
    clear_screen();
    gotoxy(col/2, 1);
    printf("You'll never win\n\r");
    tcsetattr(0, TCSAFLUSH, &old_at);
}


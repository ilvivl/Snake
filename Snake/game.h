#pragma once

#include <list>
#include <utility> // для работы с двумерными кортежами
#include <functional> //для работами с функциями, ссылками на них


using Coord = std::pair<int, int>; //2 числа //типо typedef Coord ссылка на массив

enum Dir //набор констант
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    BODY
};

enum {RABBITS_MAX = 10};

using Rabbit = Coord;


class Snake
{
public:
    Snake(); //конструктор класса
    Snake(const Snake&); //ссылка и ещё конструктор

    Snake& operator=(const Snake&); //переобозначение оператора????
    void set_dir(Dir d); //constants d
    void move(std::list<Rabbit*>& rabbits); //создание списка rabbits тип Rabbit
    Dir get_dir();
    std::list<Coord>& get_body();

private:
    std::list<Coord> body;
    Dir dir;
};


using SnakePainter  = std::function<void (Coord, Dir)>;
using RabbitPainter = std::function<void(Coord)>;


class Game //общий интерфейс игры
{
public:
    Game();
    void add(Snake* s);
    void add(Rabbit* r);
    void visit(SnakePainter sp, RabbitPainter rp);
    void update();
    void move();
    void spawn_rabbit();

private:
    std::list<Snake*>  snakes; //массив ссылок на класс
    std::list<Rabbit*> rabbits;
};


#include <functional>
#include <iostream>
#include <ctime>

#include "game.h"
#include "view.h"


Game::Game()
{
    View* v = View::get();
    v->set_model(this); //что за игра
    v->set_ontimer(350, std::bind(&Game::update, this)); //void set_ontimer(int timeout, timer_fn fn); timer принимает void и возвращает.bind подменяет аргументы функции. Теперь функция update принимает 1 аргумент вместо 0 как прежде

    srand(time(0));
    spawn_rabbit();
    spawn_rabbit();
}


void Game::visit(SnakePainter sp, RabbitPainter rp)
{
    bool f = true;
    for (auto & s : snakes) //snakes массив указателей на класс snake
    {
        auto body = s->get_body();
        Dir dir = s->get_dir();
        for (const auto& c : body)
        {
            sp(c, f ? dir : BODY); //куда дальше двигаться
            f = false;
        }
    }
    for (auto r : rabbits) //отрисовка кроликов
    {
        rp(*r); //функция координат 
    }
}


void Game::add(Snake* s)
{
    snakes.push_back(s);
}


void Game::add(Rabbit* r)
{
    rabbits.push_back(r);
}


void Game::move()
{
    for (auto s : snakes)
    {
        s->move(rabbits);
    }
}


void Game::spawn_rabbit() //границы поля
{
    int x = 2 + rand() % 100;
    int y = 2 + rand() % 30;
    Rabbit* r = new Rabbit(x, y);
    rabbits.push_back(r);  
}


void Game::update() //если мало кроликов
{
    move();
    if (rabbits.size() < 1 || (rabbits.size() <= RABBITS_MAX && rand() % 10 == 1))
        spawn_rabbit();
}


Snake::Snake() //start place
{
    body.push_back(Coord(20,15));
    body.push_back(Coord(21,15));
    body.push_back(Coord(22,15));
    body.push_back(Coord(23,15));
    body.push_back(Coord(24,15));
    dir = LEFT;
}


Snake::Snake(const Snake& s)://первая инициализация змейки
    body(s.body), dir(s.dir)
{}


Snake& Snake::operator=(const Snake& s)//обновление змейки
{
    dir = s.dir;
    body = s.body;

    return *this;
}


void Snake::set_dir(Dir d)
{
    dir = d;
}


Dir Snake::get_dir()
{
    return dir;
}


std::list<Coord>& Snake::get_body()
{
    return body;
}


void Snake::move(std::list<Rabbit*>& rabbits)
{
    auto head = body.front();
    // auto neck = snakes.get_body();

    if (this->dir == UP)// || head.second != neck.second)
        head.second--;
    else if (this->dir == LEFT)// || head.first != neck.first)
        head.first--;
    else if (this->dir == DOWN)// || head.second != neck.second)
        head.second++;
    else if (this->dir == RIGHT)// || head.first != neck.first)
        head.first++;
    else {}

    // insert(body.begin(), head);
    body.push_front(head);
    // printf("%d\n", neck);

    bool onrab = false; //ловля кролика
    for (auto r = rabbits.begin(); r != rabbits.end(); r++)
        if (head == **r)
        {
            rabbits.erase(r);
            onrab = true;
            break;
        }

    if (!onrab)
        body.pop_back();
}


#pragma once // чтобы не возникло дублирующихся объектов

class Keypressable
{
public:
    virtual void onkey(const int key) = 0; //можем переопределить
};

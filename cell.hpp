#pragma once

#include <raylib.h>
class Robot;
class Factory;

enum SURFACE
{
    FACTORY = -2,
    MOUNTAIN = -1,
    GRASS = 1,
    SAND = 2
};

class Cell
{
  private:
    Robot *robot;
    int surface;
    bool visible;

  public:
    Cell(char sym);
    int can_walk();
    int get_surface();
    Robot *get_robot();
    void set_robot(Robot *robot);
    int if_visible();
    void make_visible();
    void set_surface(int value);
};

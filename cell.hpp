#pragma once
#include <raylib.h>

class Robot;
class Factory;

enum SURFACE
{
    FACTORY,
    MOUNTAIN,
    GRASS,
    SAND
};

class Cell
{
  private:
    int surface;
    bool visible;
    bool has_item;
    bool has_robot;

  public:
    Cell(char sym);
    bool can_walk();
    int get_surface();
    bool if_visible();
    bool if_busy();
    void make_visible();
    void set_surface(int value);
    void set_busy(bool value);
};

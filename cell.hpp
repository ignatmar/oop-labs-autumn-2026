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

enum ItemType
{
    HEALTH,
    MANA,
    DAMAGE
};

class Cell
{
  private:
    Robot *robot;
    int surface;
    bool visible;
    bool has_item;
    ItemType item;

  public:
    Cell(char sym);
    bool can_walk();
    int get_surface();
    Robot *get_robot();
    void set_robot(Robot *robot);
    bool if_visible();
    void make_visible();
    void set_surface(int value);

    bool if_has_item();
    ItemType get_item();
    void set_item(ItemType item);
    void remove_item();
};

#pragma once
#include "item/item.hpp"
#include <string>
#include <vector>

class Robot;
class Factory;
class Status;

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
    bool has_robot;

    int items[ITEM_COUNT];

    std::vector<Status *> statuses;

  public:
    Cell(char sym);
    ~Cell();

    bool can_walk();
    int get_surface();
    bool if_visible();
    bool if_busy();
    void make_visible();
    void set_surface(int value);
    void set_busy(bool value);

    void add_status(Status *status);
    Status *get_status(int index);
    Status *take_status(int index);
    int get_status_count();
    std::string get_status_letters();

    void add_item(ItemType type);
    int get_item_count(ItemType type);
    void take_item(ItemType type);
    bool has_items();
};

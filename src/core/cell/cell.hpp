#pragma once
#include <raylib.h>
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
    bool has_item;
    bool has_robot;

    std::vector<Status *> statuses;

  public:
    Cell(char sym);
    ~Cell();
    Cell(const Cell &other) = delete;
    Cell &operator=(const Cell &other) = delete;
    Cell(Cell &&other);
    Cell &operator=(Cell &&other);

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
};

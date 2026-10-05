#pragma once

#include "cell.hpp"
#include <vector>
#define MAX_MAP_SIZE 100
#define MIN_MAP_SIZE 25

class Map
{
  private:
    size_t width;
    size_t height;
    int steps;
    std::vector<std::vector<Cell>> field;
    std::vector<Robot *> &enemy_robots;
    std::vector<Factory *> &factories;

  public:
    Map(size_t width, size_t height, std::vector<std::vector<char>> field,
        std::vector<Robot *> &enemy_robots, std::vector<Factory *> &factories);
    int move_robot(Robot *robot, int new_x, int new_y);
    int get_width();
    int get_height();
    bool check_coords(size_t x, size_t y);
    Cell &get_cell(int x, int y);
    const std::vector<Robot *> &get_enemy_robots();
    void set_factory(size_t x, size_t y);
    int get_steps();
    void push_enemy(Robot *robot);
    void remove_enemy(Robot *robot);
    void add_step();
    const std::vector<Factory *> &get_factories();
    void update_vision(int vision, size_t x, size_t y);
};

#pragma once

#include "cell/cell.hpp"
#include <vector>
#define MAX_MAP_SIZE 100
#define MIN_MAP_SIZE 25

class Status;

class Map
{
  private:
    size_t width;
    size_t height;
    std::vector<std::vector<Cell>> field;

    void fill_statuses();
    void fill_items();
    Status *create_status();

  public:
    Map(size_t width, size_t height, std::vector<std::vector<char>> field);
    int get_width();
    int get_height();
    bool check_coords(size_t x, size_t y);
    Cell &get_cell(int x, int y);
};

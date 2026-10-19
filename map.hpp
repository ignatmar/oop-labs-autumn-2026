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
    std::vector<std::vector<Cell>> field;

  public:
    Map(size_t width, size_t height, std::vector<std::vector<char>> field);
    int get_width();
    int get_height();
    bool check_coords(size_t x, size_t y);
    Cell &get_cell(int x, int y);
};

#include "map.hpp"
#include "cell.hpp"
#include <cstddef>
#include <cstdlib>
#include <vector>

bool Map::check_coords(size_t x, size_t y)
{
    return (y < height && x < width);
}

Map::Map(size_t width, size_t height, std::vector<std::vector<char>> field)
{
    size_t w = width;
    size_t h = height;
    if (w < MIN_MAP_SIZE)
    {
        w = MIN_MAP_SIZE;
    }
    if (w > MAX_MAP_SIZE)
    {
        w = MAX_MAP_SIZE;
    }
    if (h < MIN_MAP_SIZE)
    {
        h = MIN_MAP_SIZE;
    }
    if (h > MAX_MAP_SIZE)
    {
        h = MAX_MAP_SIZE;
    }

    this->width = w;
    this->height = h;

    this->field =
        std::vector<std::vector<Cell>>(h, std::vector<Cell>(w, Cell('.')));

    for (size_t i = 0; i < h && i < field.size(); i++)
    {
        for (size_t j = 0; j < w && j < field[0].size(); j++)
        {
            this->field[i][j] = Cell(field[i][j]);
        }
    }
}

int Map::get_width() { return (int)width; }

int Map::get_height() { return (int)height; }

Cell &Map::get_cell(int x, int y) { return field[y][x]; }

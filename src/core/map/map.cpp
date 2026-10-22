#include "map/map.hpp"
#include "cell/cell.hpp"
#include <cstddef>
#include <vector>

size_t clamp_size(size_t value)
{
    if (value < MIN_MAP_SIZE)
    {
        return MIN_MAP_SIZE;
    }
    if (value > MAX_MAP_SIZE)
    {
        return MAX_MAP_SIZE;
    }
    return value;
}

Map::Map(size_t width, size_t height, std::vector<std::vector<char>> tiles)
    : width(clamp_size(width)), height(clamp_size(height))
{
    field.clear();
    for (size_t i = 0; i < this->height; i++)
    {
        std::vector<Cell *> row;
        for (size_t j = 0; j < this->width; j++)
        {
            char symbol = '.';
            if (i < tiles.size() && j < tiles[i].size())
            {
                symbol = tiles[i][j];
            }
            row.push_back(new Cell(symbol));
        }
        field.push_back(row);
    }
}

Map::~Map()
{
    for (size_t i = 0; i < field.size(); i++)
    {
        for (size_t j = 0; j < field[i].size(); j++)
        {
            delete field[i][j];
        }
    }
}

bool Map::check_coords(size_t x, size_t y)
{
    return (y < height && x < width);
}

int Map::get_width() { return (int)width; }

int Map::get_height() { return (int)height; }

Cell &Map::get_cell(int x, int y) { return *field[y][x]; }

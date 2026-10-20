#include "map.hpp"
#include "burning.hpp"
#include "cell.hpp"
#include "overdrive.hpp"
#include "shield.hpp"
#include "slow.hpp"
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

    this->field.clear();
    for (size_t i = 0; i < h; i++)
    {
        std::vector<Cell> row;
        for (size_t j = 0; j < w; j++)
        {
            char symbol = '.';
            if (i < field.size() && j < field[i].size())
            {
                symbol = field[i][j];
            }
            row.emplace_back(symbol);
        }
        this->field.push_back(std::move(row));
    }

    fill_statuses();
}

void Map::fill_statuses()
{
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            Cell &cell = field[i][j];
            if (!cell.can_walk())
            {
                continue;
            }
            int roll = std::rand() % 100;
            int count = 0;
            if (roll < 5)
            {
                count = 2;
            }
            else if (roll < 25)
            {
                count = 1;
            }

            for (int k = 0; k < count; k++)
            {
                cell.add_status(create_status());
            }
        }
    }
}

Status *Map::create_status()
{
    switch (std::rand() % 4)
    {
    case 0:
        return new Slow();
    case 1:
        return new Overdrive();
    case 2:
        return new Burning(2, 5);
    default:
        return new Shield(20);
    }
}

int Map::get_width() { return (int)width; }

int Map::get_height() { return (int)height; }

Cell &Map::get_cell(int x, int y) { return field[y][x]; }

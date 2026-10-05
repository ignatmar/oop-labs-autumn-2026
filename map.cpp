#include "map.hpp"
#include "cell.hpp"
#include "factory.hpp"
#include "robot.hpp"
#include <cstdlib>
#include <vector>

int Map::check_coords(int x, int y)
{
    return (y < height && y >= 0 && x < width && x >= 0);
}

Map::Map(int width, int height, std::vector<std::vector<char>> field,
         std::vector<Robot *> &enemy_robots, std::vector<Factory *> &factories)
    : enemy_robots(enemy_robots), factories(factories), steps(0)
{
    if (width < MIN_MAP_SIZE)
    {
        width = MIN_MAP_SIZE;
    }
    if (width > MAX_MAP_SIZE)
    {
        width = MAX_MAP_SIZE;
    }
    if (height < MIN_MAP_SIZE)
    {
        height = MIN_MAP_SIZE;
    }
    if (height > MAX_MAP_SIZE)
    {
        height = MAX_MAP_SIZE;
    }

    this->width = width;
    this->height = height;

    this->field = std::vector<std::vector<Cell>>(
        height, std::vector<Cell>(width, Cell('.')));

    for (int i = 0; i < height && i < field.size(); i++)
    {
        for (int j = 0; j < width && j < field[0].size(); j++)
        {
            this->field[i][j] = Cell(field[i][j]);
        }
    }
}

void Map::move_robot(Robot *robot, int new_x, int new_y)
{
    if (check_coords(new_x, new_y))
    {
        Cell &new_cell = field[new_y][new_x];
        if (new_cell.can_walk())
        {
            Robot *other_robot = new_cell.get_robot();
            if (other_robot != nullptr)
            {
                robot->interact(*other_robot);
                if (other_robot->get_hp() <= 0)
                {
                    field[new_y][new_x].set_robot(nullptr);
                    robot->add_kill();
                }
                robot->set_ap(robot->get_ap() - 3);
            }
            else
            {
                field[robot->get_y()][robot->get_x()].set_robot(nullptr);
                new_cell.set_robot(robot);
                robot->set_coords(new_x, new_y);
                robot->set_ap(robot->get_ap() - new_cell.get_surface());
            }
        }
    }
}

int Map::get_width() { return width; }

int Map::get_height() { return height; }

Cell &Map::get_cell(int x, int y) { return field[y][x]; }

const std::vector<Robot *> &Map::get_enemy_robots() { return enemy_robots; }

void Map::set_factory(Factory factory, int x, int y)
{
    if (!(x > 0 && y > 0 && x < width - 1 && y < height - 1 &&
          field[y][x].get_robot() == nullptr &&
          field[y + 1][x].get_robot() == nullptr &&
          field[y][x + 1].get_robot() == nullptr &&
          field[y + 1][x + 1].get_robot() == nullptr))
    {
        return;
    }
    field[y][x].set_surface(FACTORY);
    field[y + 1][x].set_surface(FACTORY);
    field[y][x + 1].set_surface(FACTORY);
    field[y + 1][x + 1].set_surface(FACTORY);
}

void Map::push_enemy(Robot *robot) { enemy_robots.push_back(robot); }

int Map::get_steps() { return steps; }

void Map::add_step() { steps++; }

const std::vector<Factory *> &Map::get_factories() { return factories; }

void Map::update_vision(int vision, int x, int y)
{
    for (int yi = 0; yi < height; yi++)
    {
        for (int xi = 0; xi < width; xi++)
        {
            if (std::abs(x - xi) + std::abs(y - yi) <= vision)
            {
                field[yi][xi].make_visible();
            }
        }
    }
}

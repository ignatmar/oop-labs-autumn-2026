#include "map.hpp"
#include "cell.hpp"
#include "factory.hpp"
#include "robot.hpp"
#include <cstdlib>
#include <vector>

bool Map::check_coords(size_t x, size_t y) { return (y < height && x < width); }

Map::Map(size_t width, size_t height, std::vector<std::vector<char>> field,
         std::vector<Robot *> &enemy_robots, std::vector<Factory *> &factories)
    : steps(0), enemy_robots(enemy_robots), factories(factories)
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

    for (size_t i = 0; i < height && i < field.size(); i++)
    {
        for (size_t j = 0; j < width && j < field[0].size(); j++)
        {
            this->field[i][j] = Cell(field[i][j]);
        }
    }
}

int Map::move_robot(Robot *robot, int new_x, int new_y)
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
                    int levels = robot->add_exp(robot->get_max_exp());
                    TraceLog(LOG_INFO, "LEVELS: %d", levels);
                    robot->set_ap(robot->get_ap() - 3);
                    return levels;
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
    return 0;
}

int Map::get_width() { return width; }

int Map::get_height() { return height; }

Cell &Map::get_cell(int x, int y) { return field[y][x]; }

const std::vector<Robot *> &Map::get_enemy_robots() { return enemy_robots; }

void Map::set_factory(size_t x, size_t y)
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

void Map::update_vision(int vision, size_t x, size_t y)
{
    for (size_t yi = 0; yi < height; yi++)
    {
        for (size_t xi = 0; xi < width; xi++)
        {
            if (std::abs((int)(x - xi)) + std::abs(int(y - yi)) <= vision)
            {
                field[yi][xi].make_visible();
            }
        }
    }
}

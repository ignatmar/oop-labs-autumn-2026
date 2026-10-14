#include "map.hpp"
#include "cell.hpp"
#include "factory.hpp"
#include "robot.hpp"
#include <cstddef>
#include <cstdlib>
#include <vector>

bool Map::check_coords(size_t x, size_t y) { return (y < height && x < width); }

Map::Map(size_t width, size_t height, std::vector<std::vector<char>> field,
         Robot *player, std::vector<Robot *> enemy_robots,
         std::vector<Factory *> &factories)
    : steps(0), player(player), enemy_robots(enemy_robots), factories(factories)
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

int Map::move_robot(Robot *robot, int new_x, int new_y, int damage,
                    int action_points)
{
    if (!robot->can_move())
    {
        return 0;
    }

    if (check_coords(new_x, new_y))
    {
        Cell &new_cell = field[new_y][new_x];
        if (new_cell.can_walk())
        {
            Robot *other_robot = find_robot(new_x, new_y);
            if (other_robot != nullptr && other_robot->if_alive())
            {
                int levels = robot->interact(*other_robot, damage);
                robot->set_ap(robot->get_ap() - action_points);
                if (levels >= 0)
                {
                    field[new_y][new_x].set_busy(false);
                    return levels;
                }
            }
            else
            {
                field[robot->get_y()][robot->get_x()].set_busy(false);
                new_cell.set_busy(true);
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
          !field[y][x].if_busy() && !field[y + 1][x].if_busy() &&
          !field[y][x + 1].if_busy() && !field[y + 1][x + 1].if_busy()))
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

Robot *Map::find_robot(size_t x, size_t y)
{

    if (player->get_x() == x && player->get_y() == y)
    {
        return player;
    }

    for (Robot *enemy : enemy_robots)
    {
        if (x == enemy->get_x() && y == enemy->get_y())
        {
            return enemy;
        }
    }
    return nullptr;
}

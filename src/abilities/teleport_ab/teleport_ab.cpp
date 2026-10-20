#include "teleport_ab/teleport_ab.hpp"
#include "robot/robot.hpp"
#include "world/world.hpp"
#include <cstdlib>

Teleport_ab::Teleport_ab(int range, int damage, int cost)
    : range(range), damage(damage), cost(cost), level(0), unlocked(false)
{
}

void Teleport_ab::use(Robot &robot, World &world, size_t x, size_t y)
{
    if (!can_use(robot, world, x, y))
    {
        return;
    }
    robot.teleport_to(world, (int)x, (int)y);
    robot.set_mana(robot.get_mana() - cost);
    world.apply_cell_statuses(robot, (int)x, (int)y);
    world.apply_cell_items(robot, (int)x, (int)y);
}

bool Teleport_ab::can_use(Robot &robot, World &world, size_t x, size_t y)
{
    if (robot.get_hp() <= 0 || unlocked == false)
    {
        return false;
    }
    if (robot.get_mana() < cost)
    {
        return false;
    }
    return is_free_cell(robot, world, x, y);
}
bool Teleport_ab::is_free_cell(Robot &robot, World &world, size_t x, size_t y)
{
    if (!world.check_coords(x, y))
    {
        return false;
    }
    Cell &cell = world.get_cell((int)x, (int)y);
    if (cell.if_busy() && world.find_robot((int)x, (int)y) != &robot)
    {
        return false;
    }
    return in_range((int)x, (int)y, robot.get_x(), robot.get_y(), range);
}

int Teleport_ab::in_range(int x, int y, int from_x, int from_y, int range)
{
    int dx = x - from_x;
    int dy = y - from_y;
    return dx * dx + dy * dy <= range * range;
}

int Teleport_ab::get_range() { return range; }

int Teleport_ab::find_target(Robot &robot, World &world, size_t &x, size_t &y)
{
    int width = world.get_map().get_width();
    int height = world.get_map().get_height();
    for (int tries = 0; tries < 30; tries++)
    {
        size_t new_x = (size_t)(std::rand() % (size_t)width);
        size_t new_y = (size_t)(std::rand() % (size_t)height);
        if (is_free_cell(robot, world, new_x, new_y))
        {
            x = new_x;
            y = new_y;
            return 1;
        }
    }
    return 0;
}

int Teleport_ab::get_cost() { return cost; }

void Teleport_ab::upgrade()
{
    level++;
    cost--;
}

int Teleport_ab::get_level() { return level; }

bool Teleport_ab::is_unlocked() { return unlocked; }

void Teleport_ab::unlock() { unlocked = true; }

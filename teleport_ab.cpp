#include "teleport_ab.hpp"
#include "map.hpp"
#include "robot.hpp"
#include <cmath>

Teleport_ab::Teleport_ab(int range, int damage, int cost)
    : range(range), damage(damage), cost(cost)
{
}

void Teleport_ab::use(Robot &robot, Map &map, size_t x, size_t y)
{
    if (!can_use(robot, map, x, y))
    {
        return;
    }
    map.move_robot(&robot, x, y, damage, 0);
    robot.set_mana(robot.get_mana() - cost);
}

bool Teleport_ab::can_use(Robot &robot, Map &, size_t x, size_t y)
{
    if (robot.get_mana() < cost || robot.get_hp() <= 0 || unlocked == false)
    {
        return false;
    }
    if (std::pow((int)x - (int)robot.get_x(), 2) +
            std::pow((int)y - (int)robot.get_y(), 2) >
        std::pow(range, 2))
    {
        return false;
    }
    return true;
}

int Teleport_ab::get_cost() { return cost; }

void Teleport_ab::upgrade()
{
    level++;
    cost--;
}

bool Teleport_ab::is_unlocked() { return unlocked; }

void Teleport_ab::unlock() { unlocked = true; }

#include "range_ab.hpp"
#include "map.hpp"
#include "robot.hpp"
#include <cmath>

Range_ab::Range_ab(int range, int damage, int cost)
    : range(range), damage(damage), cost(cost)
{
}

void Range_ab::use(Robot &robot, Map &map, size_t x, size_t y)
{
    if (!can_use(robot, map, x, y))
    {
        return;
    }
    Robot *enemy = map.get_cell(x, y).get_robot();
    enemy->upd_hp(-damage);
    robot.set_mana(robot.get_mana() - cost);
}

bool Range_ab::can_use(Robot &robot, Map &map, size_t x, size_t y)
{
    if (robot.get_mana() < cost || robot.get_hp() <= 0)
    {
        return false;
    }
    Robot *enemy = map.get_cell(x, y).get_robot();
    if (enemy == nullptr || enemy == &robot)
    {
        return false;
    }
    if (std::pow(enemy->get_x() - robot.get_x(), 2) +
            std::pow(enemy->get_y() - robot.get_y(), 2) >
        std::pow(range, 2))
    {
        return false;
    }
    return true;
}

int Range_ab::get_cost() { return cost; }

void Range_ab::upgrade()
{
    level++;
    damage++;
}

int Range_ab::get_range() { return range; }

int Range_ab::get_damage() { return damage; }

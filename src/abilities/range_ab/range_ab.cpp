#include "range_ab/range_ab.hpp"
#include "burning/burning.hpp"
#include "robot/robot.hpp"
#include "world/world.hpp"
#include <cmath>

Range_ab::Range_ab(int range, int damage, int cost)
    : range(range), damage(damage), cost(cost), level(0), unlocked(false)
{
}

void Range_ab::use(Robot &robot, World &world, size_t x, size_t y)
{
    if (!can_use(robot, world, x, y))
    {
        return;
    }
    Robot *enemy = world.find_robot((int)x, (int)y);
    world.move_robot(&robot, (int)x, (int)y, damage, 0);
    if (enemy != nullptr)
    {
        enemy->add_status(new Burning(2, 5));
    }
    robot.set_mana(robot.get_mana() - cost);
}

bool Range_ab::can_use(Robot &robot, World &world, size_t x, size_t y)
{
    if (robot.get_mana() < cost || robot.get_hp() <= 0 || unlocked == false)
    {
        return false;
    }
    Robot *enemy = world.find_robot((int)x, (int)y);
    if (enemy == nullptr || enemy == &robot)
    {
        return false;
    }
    if (std::pow((int)enemy->get_x() - (int)robot.get_x(), 2) +
            std::pow((int)enemy->get_y() - (int)robot.get_y(), 2) >
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

int Range_ab::get_level() { return level; }

bool Range_ab::is_unlocked() { return unlocked; }
void Range_ab::unlock() { unlocked = true; }

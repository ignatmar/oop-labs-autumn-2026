#include "around_ab.hpp"
#include "map.hpp"
#include "overdrive.hpp"
#include "robot.hpp"
#include <cmath>

Around_ab::Around_ab(int radius, int damage, int cost)
    : radius(radius), damage(damage), cost(cost), unlocked(false)
{
}

void Around_ab::use(Robot &robot, Map &map, size_t, size_t)
{
    if (!can_use(robot, map, 0, 0))
    {
        return;
    }
    int xx = robot.get_x();
    int yy = robot.get_y();
    const std::vector<Robot *> &enemy_robots = map.get_enemy_robots();
    for (Robot *enemy : enemy_robots)
    {
        if (enemy == nullptr)
        {
            continue;
        }
        if (std::pow(enemy->get_x() - xx, 2) +
                std::pow(enemy->get_y() - yy, 2) <=
            pow(radius, 2))
        {
            map.move_robot(&robot, enemy->get_x(), enemy->get_y(), damage, 0);
            enemy->add_status(new Overdrive());
        }
    }
    robot.set_mana(robot.get_mana() - cost);
}

bool Around_ab::can_use(Robot &robot, Map &, size_t, size_t)
{
    if (robot.get_mana() < cost || robot.get_hp() <= 0 || unlocked == false)
    {
        return false;
    }
    return true;
}

int Around_ab::get_cost() { return cost; }

void Around_ab::upgrade()
{
    level++;
    radius++;
}

bool Around_ab::is_unlocked() { return unlocked; }

void Around_ab::unlock() { unlocked = true; }

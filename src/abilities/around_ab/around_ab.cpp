#include "around_ab/around_ab.hpp"
#include "overdrive/overdrive.hpp"
#include "robot/robot.hpp"
#include "world/world.hpp"
#include <cmath>

Around_ab::Around_ab(int radius, int damage, int cost)
    : radius(radius), damage(damage), cost(cost), level(0), unlocked(false)
{
}

void Around_ab::use(Robot &robot, World &world, size_t, size_t)
{
    if (!can_use(robot, world, 0, 0))
    {
        return;
    }
    int xx = robot.get_x();
    int yy = robot.get_y();
    std::vector<Robot *> &enemy_robots = world.get_enemies_list();
    for (Robot *enemy : enemy_robots)
    {
        if (enemy == nullptr || !enemy->if_alive())
        {
            continue;
        }
        if (std::pow(enemy->get_x() - xx, 2) +
                std::pow(enemy->get_y() - yy, 2) <=
            std::pow(radius, 2))
        {
            world.damage_robot(&robot, enemy, damage);
            if (enemy->if_alive())
            {
                enemy->add_status(new Overdrive());
            }
        }
    }
    robot.set_mana(robot.get_mana() - cost);
}

bool Around_ab::can_use(Robot &robot, World &, size_t, size_t)
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

int Around_ab::get_level() { return level; }

bool Around_ab::is_unlocked() { return unlocked; }

void Around_ab::unlock() { unlocked = true; }

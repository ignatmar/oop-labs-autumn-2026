#include "heal_ab.hpp"
#include "map.hpp"
#include "robot.hpp"

Heal_ab::Heal_ab(int range, int delta_hp, int cost)
    : range(range), delta_hp(delta_hp), cost(cost)
{
}

void Heal_ab::use(Robot &robot, Map &map, size_t x, size_t y)
{
    if (!can_use(robot, map, x, y))
    {
        return;
    }
    robot.upd_hp(delta_hp);
    robot.set_mana(robot.get_mana() - cost);
}

bool Heal_ab::can_use(Robot &robot, Map &map, size_t x, size_t y)
{
    if (robot.get_mana() < cost || robot.get_hp() <= 0)
    {
        return false;
    }
    return true;
}

int Heal_ab::get_cost() { return cost; }

void Heal_ab::upgrade()
{
    level++;
    delta_hp++;
}

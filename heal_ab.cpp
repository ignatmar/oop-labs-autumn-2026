#include "heal_ab.hpp"
#include "map.hpp"
#include "robot.hpp"
#include "shield.hpp"

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
    int extra = robot.get_hp() + delta_hp - robot.get_max_hp();

    robot.upd_hp(delta_hp);

    if (extra > 0)
    {
        robot.add_status(new Shield(extra));
    }
    robot.set_mana(robot.get_mana() - cost);
}

bool Heal_ab::can_use(Robot &robot, Map &, size_t, size_t)
{
    if (robot.get_mana() < cost || robot.get_hp() <= 0 || unlocked == false)
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

bool Heal_ab::is_unlocked() { return unlocked; }
void Heal_ab::unlock() { unlocked = true; }

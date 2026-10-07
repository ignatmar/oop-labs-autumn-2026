#include "dmg_boost.hpp"
#include "robot.hpp"

void Dmg_boost::apply(Robot &robot) { robot.set_dmg(robot.get_dmg() + damage); }

void Dmg_boost::update(Robot &robot)
{
    turns--;

    if (turns == 0)
    {
        robot.set_dmg(robot.get_dmg() - damage);
    }
}

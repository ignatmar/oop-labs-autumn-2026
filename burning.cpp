#include "burning.hpp"
#include "robot.hpp"

Burning::Burning(int turns, int damage) : turns(turns), damage(damage) {}

void Burning::apply(Robot &) {}

void Burning::update(Robot &robot)
{
    robot.upd_hp(-damage);
    turns--;
}

bool Burning::is_active() { return turns > 0; }

void Burning::combine(Status *status)
{
    Burning *burning = (Burning *)status;
    turns += burning->turns;
}

StatusType Burning::get_type() { return BURNING; }

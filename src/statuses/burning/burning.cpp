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

std::string Burning::get_label()
{
    return "BURNING x" + std::to_string(turns) + " (" +
           std::to_string(damage) + " dmg)";
}

char Burning::get_symbol() { return 'B'; }

Status *Burning::clone() { return new Burning(turns, damage); }

StatusType Burning::get_type() { return BURNING; }

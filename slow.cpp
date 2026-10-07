#include "slow.hpp"
#include "robot.hpp"

Slow::Slow() : turns(1) {}

void Slow::apply(Robot &robot) {}

void Slow::update(Robot &robot) { turns--; }

bool Slow::is_active() { return turns > 0; }

void Slow::combine(Status *status)
{
    Slow *slow = (Slow *)status;
    turns += slow->turns;
}

StatusType Slow::get_type() { return SLOW; }

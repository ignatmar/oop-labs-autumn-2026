#include "overdrive.hpp"
#include "robot.hpp"

Overdrive::Overdrive() : turns(1) {}

void Overdrive::apply(Robot &) {}

void Overdrive::update(Robot &) { turns--; }

bool Overdrive::is_active() { return turns > 0; }

void Overdrive::combine(Status *) { turns = 1; }

StatusType Overdrive::get_type() { return OVERDRIVE; }

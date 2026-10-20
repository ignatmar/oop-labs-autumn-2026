#include "slow/slow.hpp"

Slow::Slow() : turns(1), fresh(true) {}

void Slow::apply(Robot &) {}

void Slow::update(Robot &) { turns--; }

bool Slow::is_active() { return turns > 0; }

void Slow::combine(Status *status)
{
    Slow *slow = (Slow *)status;
    turns += slow->turns;
    fresh = true;
}

std::string Slow::get_label() { return "SLOW x" + std::to_string(turns); }

char Slow::get_symbol() { return 'L'; }

Status *Slow::clone() { return new Slow(); }

bool Slow::is_fresh() { return fresh; }

void Slow::make_ready() { fresh = false; }

StatusType Slow::get_type() { return SLOW; }

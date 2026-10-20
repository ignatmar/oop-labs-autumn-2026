#include "overdrive/overdrive.hpp"

Overdrive::Overdrive() : turns(1) {}

void Overdrive::apply(Robot &) {}

void Overdrive::update(Robot &) { turns--; }

bool Overdrive::is_active() { return turns > 0; }

void Overdrive::combine(Status *) { turns = 1; }

std::string Overdrive::get_label() { return "OVERDRIVE"; }

char Overdrive::get_symbol() { return 'O'; }

Status *Overdrive::clone() { return new Overdrive(); }

StatusType Overdrive::get_type() { return OVERDRIVE; }

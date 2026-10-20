#include "shield.hpp"
#include "robot.hpp"

Shield::Shield(int damage) : turns(0), damage(damage) {}

void Shield::apply(Robot &) {}

void Shield::update(Robot &) {}

bool Shield::is_active() { return damage > 0; }

StatusType Shield::get_type() { return SHIELD; }

void Shield::combine(Status *status)
{
    Shield *shield = (Shield *)status;
    damage += shield->damage;
}

int Shield::get_damage() { return damage; }

void Shield::set_damage(int damage) { this->damage = damage; }

std::string Shield::get_label()
{
    return "SHIELD " + std::to_string(damage);
}

char Shield::get_symbol() { return 'S'; }

Status *Shield::clone() { return new Shield(damage); }

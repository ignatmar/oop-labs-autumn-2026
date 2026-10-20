#include "attack_boost/attack_boost.hpp"

AttackBoost::AttackBoost(int bonus) : bonus(bonus), turns(1) {}

void AttackBoost::apply(Robot &) {}

void AttackBoost::update(Robot &) { turns--; }

bool AttackBoost::is_active() { return turns > 0; }

void AttackBoost::combine(Status *status)
{
    AttackBoost *other = (AttackBoost *)status;
    if (other->bonus > bonus)
    {
        bonus = other->bonus;
    }
    turns = 1;
}

std::string AttackBoost::get_label() { return "ATTACK +" + std::to_string(bonus); }

char AttackBoost::get_symbol() { return 'A'; }

Status *AttackBoost::clone() { return new AttackBoost(bonus); }

int AttackBoost::get_bonus() { return bonus; }

StatusType AttackBoost::get_type() { return ATTACK_BOOST; }

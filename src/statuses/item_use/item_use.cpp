#include "item_use.hpp"
#include "robot.hpp"

ItemUse::ItemUse() : turns(1) {}

void ItemUse::apply(Robot &robot) { robot.use_item(); }

void ItemUse::update(Robot &) { turns--; }

bool ItemUse::is_active() { return turns > 0; }

void ItemUse::combine(Status *status) { turns += ((ItemUse *)status)->turns; }

std::string ItemUse::get_label()
{
    return "ITEM USE x" + std::to_string(turns);
}

char ItemUse::get_symbol() { return 'U'; }

Status *ItemUse::clone() { return new ItemUse(); }

StatusType ItemUse::get_type() { return ITEM_USE; }

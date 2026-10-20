#include "item_take.hpp"
#include "robot.hpp"

ItemTake::ItemTake() : turns(1) {}

void ItemTake::apply(Robot &robot) { robot.add_item(1); }

void ItemTake::update(Robot &) { turns--; }

bool ItemTake::is_active() { return turns > 0; }

void ItemTake::combine(Status *status) { turns += ((ItemTake *)status)->turns; }

std::string ItemTake::get_label()
{
    return "ITEM TAKE x" + std::to_string(turns);
}

char ItemTake::get_symbol() { return 'T'; }

Status *ItemTake::clone() { return new ItemTake(); }

StatusType ItemTake::get_type() { return ITEM_TAKE; }

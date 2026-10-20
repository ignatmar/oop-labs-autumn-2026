#include "item_take/item_take.hpp"
#include "robot/robot.hpp"

ItemTake::ItemTake(ItemType type, int count)
    : type(type), count(count), turns(1)
{
}

void ItemTake::apply(Robot &robot) { robot.add_item(type, count); }

void ItemTake::update(Robot &) { turns--; }

bool ItemTake::is_active() { return turns > 0; }

void ItemTake::combine(Status *status)
{
    ItemTake *other = (ItemTake *)status;
    count += other->count;
    turns += other->turns;
}

std::string ItemTake::get_label()
{
    // return "TOOK " + std::string(item_name(type)) + " x" +
    // std::to_string(count);
    return "";
}

char ItemTake::get_symbol() { return 'T'; }

Status *ItemTake::clone() { return new ItemTake(type, count); }

StatusType ItemTake::get_type() { return ITEM_TAKE; }

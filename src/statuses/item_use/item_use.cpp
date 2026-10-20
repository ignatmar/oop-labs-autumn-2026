#include "item_use/item_use.hpp"
#include "robot/robot.hpp"

ItemUse::ItemUse(ItemType type) : type(type), turns(1) {}

void ItemUse::apply(Robot &robot) { robot.use_item(type); }

void ItemUse::update(Robot &) { turns--; }

bool ItemUse::is_active() { return turns > 0; }

void ItemUse::combine(Status *status)
{
    ItemUse *other = (ItemUse *)status;
    turns += other->turns;
}

std::string ItemUse::get_label()
{
    // return "USED " + std::string(item_name(type));
    return "";
}

char ItemUse::get_symbol() { return 'U'; }

Status *ItemUse::clone() { return new ItemUse(type); }

StatusType ItemUse::get_type() { return ITEM_USE; }

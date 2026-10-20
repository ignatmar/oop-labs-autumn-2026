#pragma once

enum ItemType
{
    ITEM_HEALTH = 0,
    ITEM_MANA = 1,
    ITEM_ATTACK = 2,
    ITEM_COUNT = 3
};

inline const char *item_name(ItemType type)
{
    switch (type)
    {
    case ITEM_HEALTH:
        return "HEALTH";
    case ITEM_MANA:
        return "MANA";
    case ITEM_ATTACK:
        return "ATTACK";
    default:
        return "ITEM";
    }
}

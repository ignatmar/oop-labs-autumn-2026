#pragma once
#include "item/item.hpp"
#include <cstddef>

struct PlayerSpawn
{
    int x = 1;
    int y = 1;
    int hp_max = 3000;
    int dmg = 30;
    int mana_max = 500;
    int speed = 7;
    int exp_max = 100;
    int vision = 100;
};

struct PlayerProfile
{
    int hp = 3000;
    int hp_max = 3000;
    int dmg = 30;
    int mana_max = 500;
    int speed = 7;
    int exp = 0;
    int exp_max = 100;
    int lvl = 0;
    int vision = 100;
    size_t kills = 0;
    int items[ITEM_COUNT] = {};

    void from_spawn(PlayerSpawn &spawn)
    {
        hp = spawn.hp_max;
        hp_max = spawn.hp_max;
        dmg = spawn.dmg;
        mana_max = spawn.mana_max;
        speed = spawn.speed;
        exp = 0;
        exp_max = spawn.exp_max;
        lvl = 0;
        vision = spawn.vision;
        kills = 0;
        for (int i = 0; i < ITEM_COUNT; i++)
        {
            items[i] = 0;
        }
    }
};

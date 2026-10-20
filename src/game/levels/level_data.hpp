#pragma once
#include "item/item.hpp"
#include "player/profile.hpp"
#include "status.hpp"
#include <vector>

enum GameMode
{
    MODE_FIXED = 0,
    MODE_RANDOM = 1,
    MODE_ENDLESS = 2
};

struct EnemySpawn
{
    int x = 1;
    int y = 1;
    int rank = 1;
};

struct FactorySpawn
{
    int x = 1;
    int y = 1;
    int freq = 3;
    int rank = 1;
};

struct StatusSpawn
{
    int x = 0;
    int y = 0;
    StatusType type = SLOW;
    int turns = 1;
    int value = 0;
};

struct ItemSpawn
{
    int x = 0;
    int y = 0;
    ItemType type = ITEM_HEALTH;
};

struct LevelData
{
    int width = 25;
    int height = 25;
    std::vector<std::vector<char>> tiles;
    PlayerSpawn player;
    std::vector<EnemySpawn> enemies;
    std::vector<FactorySpawn> factories;
    std::vector<StatusSpawn> statuses;
    std::vector<ItemSpawn> items;
};

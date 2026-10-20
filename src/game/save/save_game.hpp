#pragma once
#include "levels/level_data.hpp"
#include <string>

struct AbilityState
{
    bool unlocked = false;
    int level = 0;
};

struct SaveData
{
    GameMode mode = MODE_FIXED;
    int current_level = 0;
    unsigned seed = 0;
    PlayerProfile profile;
    AbilityState abilities[4];
};

class SaveGame
{
  public:
    static bool write(std::string path, SaveData &data);
    static bool read(std::string path, SaveData &data);
    static bool exists(std::string path);
};

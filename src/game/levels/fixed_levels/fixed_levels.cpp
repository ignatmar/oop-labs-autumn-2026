#include "levels/fixed_levels/fixed_levels.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <string>
#include <vector>

StatusType parse_status(std::string value)
{
    if (value == "overdrive")
    {
        return OVERDRIVE;
    }
    if (value == "burning")
    {
        return BURNING;
    }
    if (value == "shield")
    {
        return SHIELD;
    }
    if (value == "attack_boost")
    {
        return ATTACK_BOOST;
    }
    return SLOW;
}

ItemType parse_item(std::string value)
{
    if (value == "mana")
    {
        return ITEM_MANA;
    }
    if (value == "attack")
    {
        return ITEM_ATTACK;
    }
    return ITEM_HEALTH;
}

FixedLevels::FixedLevels(std::string directory)
{
    DIR *dir = opendir(directory.c_str());
    if (dir == nullptr)
    {
        return;
    }

    std::vector<std::string> files;
    struct dirent *entry = nullptr;
    while ((entry = readdir(dir)) != nullptr)
    {
        std::string name = entry->d_name;
        if (name.size() <= 4 || name.substr(name.size() - 4) != ".txt")
        {
            continue;
        }
        std::string path = directory;
        if (!path.empty() && path.back() != '/')
        {
            path += "/";
        }
        path += name;
        files.push_back(path);
    }
    closedir(dir);

    for (size_t i = 1; i < files.size(); i++)
    {
        std::string key = files[i];
        size_t j = i;
        while (j > 0 && files[j - 1] > key)
        {
            files[j] = files[j - 1];
            j--;
        }
        files[j] = key;
    }

    for (size_t i = 0; i < files.size(); i++)
    {
        load_file(files[i]);
    }
}

void FixedLevels::load_file(std::string path)
{
    std::ifstream in(path);
    if (!in.is_open())
    {
        return;
    }

    LevelData data;
    data.width = 25;
    data.height = 25;

    std::string line;
    bool map_started = false;
    std::vector<std::vector<char>> tiles;

    while (std::getline(in, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (map_started)
        {
            tiles.push_back(std::vector<char>(line.begin(), line.end()));
            continue;
        }

        char command[64] = {};
        if (std::sscanf(line.c_str(), "%63s", command) != 1)
        {
            continue;
        }

        if (std::strcmp(command, "size") == 0)
        {
            std::sscanf(line.c_str(), "%*s %d %d", &data.width, &data.height);
        }
        else if (std::strcmp(command, "player") == 0)
        {
            std::sscanf(line.c_str(), "%*s %d %d %d %d %d %d %d %d",
                        &data.player.x, &data.player.y, &data.player.hp_max,
                        &data.player.dmg, &data.player.mana_max,
                        &data.player.speed, &data.player.exp_max,
                        &data.player.vision);
        }
        else if (std::strcmp(command, "enemy") == 0)
        {
            EnemySpawn spawn;
            std::sscanf(line.c_str(), "%*s %d %d %d", &spawn.x, &spawn.y,
                        &spawn.rank);
            data.enemies.push_back(spawn);
        }
        else if (std::strcmp(command, "factory") == 0)
        {
            FactorySpawn spawn;
            std::sscanf(line.c_str(), "%*s %d %d %d %d", &spawn.x, &spawn.y,
                        &spawn.freq, &spawn.rank);
            data.factories.push_back(spawn);
        }
        else if (std::strcmp(command, "status") == 0)
        {
            StatusSpawn spawn;
            char type[64] = {};
            std::sscanf(line.c_str(), "%*s %d %d %63s %d %d", &spawn.x,
                        &spawn.y, type, &spawn.turns, &spawn.value);
            spawn.type = parse_status(type);
            data.statuses.push_back(spawn);
        }
        else if (std::strcmp(command, "item") == 0)
        {
            ItemSpawn spawn;
            char type[64] = {};
            std::sscanf(line.c_str(), "%*s %d %d %63s", &spawn.x, &spawn.y,
                        type);
            spawn.type = parse_item(type);
            data.items.push_back(spawn);
        }
        else if (std::strcmp(command, "map") == 0)
        {
            map_started = true;
        }
    }

    data.tiles = tiles;
    levels.push_back(data);
}

int FixedLevels::count()
{
    return (int)levels.size();
}

bool FixedLevels::has_next(int index)
{
    return index + 1 < (int)levels.size();
}

LevelData FixedLevels::make_level(int index)
{
    if (index < 0 || index >= (int)levels.size())
    {
        return LevelData();
    }
    return levels[index];
}

GameMode FixedLevels::mode()
{
    return MODE_FIXED;
}

unsigned FixedLevels::get_seed()
{
    return 0;
}

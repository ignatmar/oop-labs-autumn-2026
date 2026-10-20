#include "levels/fixed_levels/fixed_levels.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

static StatusType parse_status(std::string value)
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

static ItemType parse_item(std::string value)
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
    std::error_code error;
    std::filesystem::path dir(directory);
    if (!std::filesystem::exists(dir, error))
    {
        return;
    }

    std::vector<std::string> files;
    for (std::filesystem::directory_iterator it(dir, error), end; it != end;
         it.increment(error))
    {
        if (error)
        {
            break;
        }
        if (it->path().extension() == ".txt")
        {
            files.push_back(it->path().string());
        }
    }
    std::sort(files.begin(), files.end());

    for (std::string &file : files)
    {
        load_file(file);
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

        std::istringstream stream(line);
        std::string command;
        stream >> command;
        if (command.empty())
        {
            continue;
        }

        if (command == "size")
        {
            stream >> data.width >> data.height;
        }
        else if (command == "player")
        {
            stream >> data.player.x >> data.player.y >> data.player.hp_max >>
                data.player.dmg >> data.player.mana_max >> data.player.speed >>
                data.player.exp_max >> data.player.vision;
        }
        else if (command == "enemy")
        {
            EnemySpawn spawn;
            stream >> spawn.x >> spawn.y >> spawn.rank;
            data.enemies.push_back(spawn);
        }
        else if (command == "factory")
        {
            FactorySpawn spawn;
            stream >> spawn.x >> spawn.y >> spawn.freq >> spawn.rank;
            data.factories.push_back(spawn);
        }
        else if (command == "status")
        {
            StatusSpawn spawn;
            std::string type;
            stream >> spawn.x >> spawn.y >> type;
            spawn.type = parse_status(type);
            stream >> spawn.turns >> spawn.value;
            data.statuses.push_back(spawn);
        }
        else if (command == "item")
        {
            ItemSpawn spawn;
            std::string type;
            stream >> spawn.x >> spawn.y >> type;
            spawn.type = parse_item(type);
            data.items.push_back(spawn);
        }
        else if (command == "map")
        {
            map_started = true;
        }
    }

    data.tiles = std::move(tiles);
    levels.push_back(std::move(data));
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

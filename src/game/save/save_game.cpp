#include "save/save_game.hpp"
#include <fstream>
#include <string>

bool SaveGame::write(std::string path, SaveData &data)
{
    std::ofstream out(path);
    if (!out.is_open())
    {
        return false;
    }

    out << "mode " << (int)data.mode << "\n";
    out << "level " << data.current_level << "\n";
    out << "seed " << data.seed << "\n";
    out << "player " << data.profile.hp << " " << data.profile.hp_max << " "
        << data.profile.dmg << " " << data.profile.mana_max << " "
        << data.profile.speed << " " << data.profile.exp << " "
        << data.profile.exp_max << " " << data.profile.lvl << " "
        << data.profile.vision << " " << data.profile.kills << "\n";
    out << "items " << data.profile.items[ITEM_HEALTH] << " "
        << data.profile.items[ITEM_MANA] << " "
        << data.profile.items[ITEM_ATTACK] << "\n";
    for (int i = 0; i < 4; i++)
    {
        out << "ability " << i << " " << data.abilities[i].unlocked << " "
            << data.abilities[i].level << "\n";
    }
    return true;
}

bool SaveGame::read(std::string path, SaveData &data)
{
    std::ifstream in(path);
    if (!in.is_open())
    {
        return false;
    }

    SaveData loaded;
    std::string key;
    while (in >> key)
    {
        if (key == "mode")
        {
            int mode = -1;
            in >> mode;
            if (mode < MODE_FIXED || mode > MODE_ENDLESS)
            {
                return false;
            }
            loaded.mode = (GameMode)mode;
        }
        else if (key == "level")
        {
            in >> loaded.current_level;
        }
        else if (key == "seed")
        {
            in >> loaded.seed;
        }
        else if (key == "player")
        {
            in >> loaded.profile.hp >> loaded.profile.hp_max >>
                loaded.profile.dmg >> loaded.profile.mana_max >>
                loaded.profile.speed >> loaded.profile.exp >>
                loaded.profile.exp_max >> loaded.profile.lvl >>
                loaded.profile.vision >> loaded.profile.kills;
        }
        else if (key == "items")
        {
            in >> loaded.profile.items[ITEM_HEALTH] >>
                loaded.profile.items[ITEM_MANA] >>
                loaded.profile.items[ITEM_ATTACK];
        }
        else if (key == "ability")
        {
            int index = -1;
            in >> index;
            if (index < 0 || index >= 4)
            {
                return false;
            }
            in >> loaded.abilities[index].unlocked >>
                loaded.abilities[index].level;
        }
        else
        {
            std::string rest;
            std::getline(in, rest);
        }
    }

    if (!in.eof())
    {
        return false;
    }
    if (loaded.current_level < 0)
    {
        return false;
    }

    data = loaded;
    return true;
}

bool SaveGame::exists(std::string path)
{
    std::ifstream in(path);
    return in.is_open();
}

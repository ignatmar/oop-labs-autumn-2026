#include "levels/level.hpp"
#include "attack_boost/attack_boost.hpp"
#include "burning/burning.hpp"
#include "factory/factory.hpp"
#include "overdrive/overdrive.hpp"
#include "shield/shield.hpp"
#include "slow/slow.hpp"

Level::Level(LevelData &data, PlayerProfile &profile)
    : player(new Robot(profile.hp_max, profile.dmg, profile.mana_max,
                       profile.speed, profile.exp_max, 1, profile.vision,
                       true)),
      world(new World(new Map(data.width, data.height, data.tiles)))
{
    for (FactorySpawn &spawn : data.factories)
    {
        int rank = spawn.rank < 1 ? 1 : spawn.rank;
        world->add_factory(new Factory(spawn.x, spawn.y, spawn.freq, rank));
        world->set_factory((size_t)spawn.x, (size_t)spawn.y);
    }

    player->set_hp(profile.hp);
    player->set_exp(profile.exp);
    player->set_lvl(profile.lvl);
    for (size_t i = 0; i < profile.kills; i++)
    {
        player->add_kill();
    }
    for (int i = 0; i < ITEM_COUNT; i++)
    {
        if (profile.items[i] > 0)
        {
            player->add_item((ItemType)i, profile.items[i]);
        }
    }

    int spawn_x = data.player.x;
    int spawn_y = data.player.y;
    if (!world->check_coords((size_t)spawn_x, (size_t)spawn_y) ||
        !world->get_cell(spawn_x, spawn_y).can_walk() ||
        world->get_cell(spawn_x, spawn_y).if_busy())
    {
        spawn_x = -1;
        spawn_y = -1;
        for (int y = 0; y < data.height && spawn_x < 0; y++)
        {
            for (int x = 0; x < data.width; x++)
            {
                Cell &cell = world->get_cell(x, y);
                if (cell.can_walk() && !cell.if_busy())
                {
                    spawn_x = x;
                    spawn_y = y;
                    break;
                }
            }
        }
    }

    if (spawn_x >= 0)
    {
        player->set_coords(spawn_x, spawn_y);
        world->set_player(player);
    }

    for (EnemySpawn &spawn : data.enemies)
    {
        if (!world->check_coords((size_t)spawn.x, (size_t)spawn.y))
        {
            continue;
        }
        Cell &cell = world->get_cell(spawn.x, spawn.y);
        if (!cell.can_walk() || cell.if_busy())
        {
            continue;
        }

        int rank = spawn.rank < 1 ? 1 : spawn.rank;
        Robot *enemy = new Robot(80 + rank * 40, 10 + rank * 2,
                                 50 + rank * 10, 6, 100 + rank * 20, 2, 10,
                                 true);
        enemy->set_lvl(rank);
        enemy->set_coords(spawn.x, spawn.y);
        world->add_enemy(enemy);
    }

    for (StatusSpawn &spawn : data.statuses)
    {
        if (!world->check_coords((size_t)spawn.x, (size_t)spawn.y))
        {
            continue;
        }
        Cell &cell = world->get_cell(spawn.x, spawn.y);
        if (!cell.can_walk())
        {
            continue;
        }
        Status *status = create_status(spawn);
        if (status != nullptr)
        {
            cell.add_status(status);
        }
    }

    for (ItemSpawn &spawn : data.items)
    {
        if (!world->check_coords((size_t)spawn.x, (size_t)spawn.y))
        {
            continue;
        }
        Cell &cell = world->get_cell(spawn.x, spawn.y);
        if (cell.can_walk())
        {
            cell.add_item(spawn.type);
        }
    }
}

Status *Level::create_status(StatusSpawn &spawn)
{
    if (spawn.type == SLOW)
    {
        return new Slow();
    }
    if (spawn.type == OVERDRIVE)
    {
        return new Overdrive();
    }
    if (spawn.type == BURNING)
    {
        int turns = spawn.turns > 0 ? spawn.turns : 2;
        int damage = spawn.value > 0 ? spawn.value : 5;
        return new Burning(turns, damage);
    }
    if (spawn.type == SHIELD)
    {
        int damage = spawn.value > 0 ? spawn.value : 20;
        return new Shield(damage);
    }
    if (spawn.type == ATTACK_BOOST)
    {
        int bonus = spawn.value > 0 ? spawn.value : 10;
        return new AttackBoost(bonus);
    }
    return nullptr;
}

Level::~Level()
{
    delete world;
    delete player;
}

World &Level::get_world()
{
    return *world;
}

Robot &Level::get_player()
{
    return *player;
}

PlayerProfile Level::make_profile()
{
    PlayerProfile profile;
    profile.hp = player->get_hp();
    profile.hp_max = player->get_max_hp();
    profile.dmg = player->get_base_dmg();
    profile.mana_max = player->get_max_mana();
    profile.speed = player->get_speed();
    profile.exp = player->get_exp();
    profile.exp_max = player->get_max_exp();
    profile.lvl = player->get_lvl();
    profile.vision = player->get_vision();
    profile.kills = player->get_kills();
    for (int i = 0; i < ITEM_COUNT; i++)
    {
        profile.items[i] = player->get_item_count((ItemType)i);
    }
    return profile;
}

#include "world.hpp"
#include <cstdlib>

World::World(Map &map)
    : map(map), player(nullptr), steps(0)
{
}

void World::set_player(Robot *robot)
{
    player = robot;
    if (player != nullptr)
    {
        map.get_cell(player->get_x(), player->get_y()).set_busy(true);
    }
}

void World::add_enemy(Robot *robot)
{
    if (robot == nullptr)
    {
        return;
    }
    enemies.push_back(robot);
    map.get_cell(robot->get_x(), robot->get_y()).set_busy(true);
}

void World::add_factory(Factory *factory)
{
    if (factory == nullptr)
    {
        return;
    }
    factories.push_back(factory);
}

std::vector<Factory *> &World::get_factories()
{
    return factories;
}

Map &World::get_map()
{
    return map;
}

Robot *World::find_robot(int x, int y)
{
    if (player != nullptr && player->get_x() == x && player->get_y() == y)
    {
        return player;
    }
    for (Robot *enemy : enemies)
    {
        if (enemy == nullptr)
        {
            continue;
        }
        if (enemy->get_x() == x && enemy->get_y() == y)
        {
            return enemy;
        }
    }
    return nullptr;
}

int World::move_robot(Robot *robot, int new_x, int new_y, int damage, int action_points)
{
    if (robot == nullptr || !robot->can_move())
    {
        return 0;
    }

    if (check_coords((size_t)new_x, (size_t)new_y))
    {
        Cell &new_cell = map.get_cell(new_x, new_y);
        if (new_cell.can_walk())
        {
            Robot *other_robot = find_robot(new_x, new_y);
            if (other_robot != nullptr && other_robot->if_alive())
            {
                int levels = robot->interact(*other_robot, damage);
                robot->set_ap(robot->get_ap() - action_points);
                if (levels >= 0)
                {
                    map.get_cell(new_x, new_y).set_busy(false);
                    return levels;
                }
            }
            else
            {
                map.get_cell(robot->get_y(), robot->get_x()).set_busy(false);
                new_cell.set_busy(true);
                robot->set_coords(new_x, new_y);
                robot->set_ap(robot->get_ap() - new_cell.get_surface());
            }
        }
    }
    return 0;
}

int World::move_player(int dx, int dy, int action_points)
{
    if (player == nullptr)
    {
        return 0;
    }
    return move_robot(player, player->get_x() + dx, player->get_y() + dy,
                      player->get_dmg(), action_points);
}

void World::update_vision()
{
    if (player == nullptr)
    {
        return;
    }
    int vision = player->get_vision();
    int x = player->get_x();
    int y = player->get_y();
    for (int yi = 0; yi < map.get_height(); yi++)
    {
        for (int xi = 0; xi < map.get_width(); xi++)
        {
            if (std::abs(x - xi) + std::abs(y - yi) <= vision)
            {
                map.get_cell(xi, yi).make_visible();
            }
        }
    }
}

void World::set_factory(size_t x, size_t y)
{
    if (!(x > 0 && y > 0 && x < (size_t)map.get_width() - 1 &&
          y < (size_t)map.get_height() - 1))
    {
        return;
    }
    if (map.get_cell((int)x, (int)y).if_busy() ||
        map.get_cell((int)x + 1, (int)y).if_busy() ||
        map.get_cell((int)x, (int)y + 1).if_busy() ||
        map.get_cell((int)x + 1, (int)y + 1).if_busy())
    {
        return;
    }
    map.get_cell((int)x, (int)y).set_surface(FACTORY);
    map.get_cell((int)x + 1, (int)y).set_surface(FACTORY);
    map.get_cell((int)x, (int)y + 1).set_surface(FACTORY);
    map.get_cell((int)x + 1, (int)y + 1).set_surface(FACTORY);
}

void World::spawn_from_factories()
{
    for (Factory *factory : factories)
    {
        if (factory != nullptr)
        {
            factory->spawn(*this);
        }
    }
}

bool World::player_alive()
{
    return player != nullptr && player->get_hp() > 0;
}

void World::player_update_statuses()
{
    if (player != nullptr)
    {
        player->update_statuses();
    }
}

bool World::player_can_use_ability()
{
    return player != nullptr && player->can_use_ability();
}

int World::get_player_dmg()
{
    return player != nullptr ? player->get_dmg() : 0;
}

void World::refill_ap()
{
    for (Robot *enemy : enemies)
    {
        if (enemy != nullptr)
        {
            enemy->set_ap(enemy->get_max_ap());
        }
    }
    if (player != nullptr)
    {
        player->set_ap(player->get_max_ap());
    }
}

bool World::is_player_turn_over()
{
    return player == nullptr || player->get_ap() <= 0;
}

Robot &World::get_player_ref()
{
    return *player;
}

std::vector<Robot *> &World::get_enemies_list()
{
    return enemies;
}

void World::add_step()
{
    steps++;
}

void World::process_enemy_turns(double &timer)
{
    for (Robot *enemy : enemies)
    {
        if (enemy == nullptr || enemy->get_hp() <= 0 || enemy->get_ap() <= 0)
        {
            continue;
        }
        int e_x = enemy->get_x();
        int e_y = enemy->get_y();
        int p_x = player != nullptr ? player->get_x() : e_x;
        int p_y = player != nullptr ? player->get_y() : e_y;
        bool moved = false;
        if (e_x < p_x)
        {
            move_robot(enemy, e_x + 1, e_y, enemy->get_dmg(), 3);
            moved = (enemy->get_x() != e_x || enemy->get_y() != e_y);
        }
        if (!moved && e_y < p_y)
        {
            move_robot(enemy, e_x, e_y + 1, enemy->get_dmg(), 3);
            moved = (enemy->get_x() != e_x || enemy->get_y() != e_y);
        }
        if (!moved && e_x > p_x)
        {
            move_robot(enemy, e_x - 1, e_y, enemy->get_dmg(), 3);
            moved = (enemy->get_x() != e_x || enemy->get_y() != e_y);
        }
        if (!moved && e_y > p_y)
        {
            move_robot(enemy, e_x, e_y - 1, enemy->get_dmg(), 3);
        }
        enemy->update_statuses();
        timer = 0;
        break;
    }
}

bool World::has_enemies_with_ap()
{
    for (Robot *enemy : enemies)
    {
        if (enemy != nullptr && enemy->get_hp() > 0 && enemy->get_ap() > 0)
        {
            return true;
        }
    }
    return false;
}

int World::get_steps()
{
    return steps;
}

bool World::all_enemies_dead()
{
    for (Robot *enemy : enemies)
    {
        if (enemy != nullptr && enemy->if_alive())
        {
            return false;
        }
    }
    return true;
}

bool World::check_coords(size_t x, size_t y)
{
    return map.check_coords(x, y);
}

Cell &World::get_cell(int x, int y)
{
    return map.get_cell(x, y);
}

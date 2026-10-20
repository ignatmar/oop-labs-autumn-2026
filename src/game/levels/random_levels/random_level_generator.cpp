#include "levels/random_levels/random_level_generator.hpp"
#include <cstddef>
#include <queue>
#include <random>
#include <utility>
#include <vector>

RandomLevelGenerator::RandomLevelGenerator(unsigned seed) : seed(seed) {}

static bool reachable_from_player(LevelData &data,
                                  std::vector<std::vector<bool>> &factory_cells)
{
    std::vector<std::vector<bool>> visited(
        data.height, std::vector<bool>(data.width, false));
    std::queue<std::pair<int, int>> queue;
    queue.push({data.player.x, data.player.y});
    visited[data.player.y][data.player.x] = true;

    int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    while (!queue.empty())
    {
        std::pair<int, int> current = queue.front();
        queue.pop();
        for (int i = 0; i < 4; i++)
        {
            int x = current.first + directions[i][0];
            int y = current.second + directions[i][1];
            if (x < 0 || y < 0 || x >= data.width || y >= data.height)
            {
                continue;
            }
            if (visited[y][x] || factory_cells[y][x] ||
                data.tiles[y][x] == 'M')
            {
                continue;
            }
            visited[y][x] = true;
            queue.push({x, y});
        }
    }

    for (EnemySpawn &enemy : data.enemies)
    {
        if (!visited[enemy.y][enemy.x])
        {
            return false;
        }
    }

    for (FactorySpawn &factory : data.factories)
    {
        bool has_access = false;
        for (int dy = -1; dy <= 2 && !has_access; dy++)
        {
            for (int dx = -1; dx <= 2; dx++)
            {
                if (dx >= 0 && dx <= 1 && dy >= 0 && dy <= 1)
                {
                    continue;
                }
                int x = factory.x + dx;
                int y = factory.y + dy;
                if (x < 0 || y < 0 || x >= data.width || y >= data.height)
                {
                    continue;
                }
                if (!factory_cells[y][x] && data.tiles[y][x] != 'M' &&
                    visited[y][x])
                {
                    has_access = true;
                    break;
                }
            }
        }
        if (!has_access)
        {
            return false;
        }
    }
    return true;
}

LevelData RandomLevelGenerator::make(int index)
{
    std::mt19937 rng(seed + (unsigned)index * 7919u);

    for (int attempt = 0; attempt < 30; attempt++)
    {
        LevelData data;
        data.width = 25;
        data.height = 25;
        data.player.x = 1;
        data.player.y = 1;
        data.player.hp_max = 3000;
        data.player.dmg = 30;
        data.player.mana_max = 500;
        data.player.speed = 7;
        data.player.exp_max = 100;
        data.player.vision = 100;

        int obstacle_chance = 6 + index;
        if (obstacle_chance > 15)
        {
            obstacle_chance = 15;
        }

        data.tiles.assign(data.height, std::vector<char>(data.width, '.'));
        for (int y = 1; y < data.height - 1; y++)
        {
            for (int x = 1; x < data.width - 1; x++)
            {
                int roll = (int)(rng() % 100);
                if (roll < obstacle_chance)
                {
                    data.tiles[y][x] = 'M';
                }
                else if (roll < obstacle_chance * 2)
                {
                    data.tiles[y][x] = 'S';
                }
            }
        }

        for (int y = 0; y < 3; y++)
        {
            for (int x = 0; x < 3; x++)
            {
                data.tiles[y][x] = '.';
            }
        }

        std::vector<std::vector<bool>> factory_cells(
            data.height, std::vector<bool>(data.width, false));
        std::vector<std::vector<bool>> used(
            data.height, std::vector<bool>(data.width, false));
        used[data.player.y][data.player.x] = true;

        int rank = 2 + index;
        int factory_count = 1 + index / 3;
        if (factory_count > 3)
        {
            factory_count = 3;
        }

        for (int i = 0; i < factory_count; i++)
        {
            bool placed = false;
            for (int tries = 0; tries < 100 && !placed; tries++)
            {
                int fx = 1 + (int)(rng() % (unsigned)(data.width - 3));
                int fy = 1 + (int)(rng() % (unsigned)(data.height - 3));
                bool free = true;
                for (int dy = 0; dy < 2 && free; dy++)
                {
                    for (int dx = 0; dx < 2; dx++)
                    {
                        if (data.tiles[fy + dy][fx + dx] != '.' ||
                            factory_cells[fy + dy][fx + dx] ||
                            used[fy + dy][fx + dx])
                        {
                            free = false;
                            break;
                        }
                    }
                }
                if (!free)
                {
                    continue;
                }

                FactorySpawn factory;
                factory.x = fx;
                factory.y = fy;
                factory.freq = 3;
                factory.rank = rank;
                data.factories.push_back(factory);
                for (int dy = 0; dy < 2; dy++)
                {
                    for (int dx = 0; dx < 2; dx++)
                    {
                        factory_cells[fy + dy][fx + dx] = true;
                    }
                }
                placed = true;
            }
        }

        int enemy_count = 2 + index;
        if (enemy_count > 8)
        {
            enemy_count = 8;
        }

        for (int i = 0; i < enemy_count; i++)
        {
            bool placed = false;
            for (int tries = 0; tries < 200 && !placed; tries++)
            {
                int x = (int)(rng() % (unsigned)data.width);
                int y = (int)(rng() % (unsigned)data.height);
                if (data.tiles[y][x] != '.' || factory_cells[y][x] ||
                    used[y][x])
                {
                    continue;
                }
                EnemySpawn enemy;
                enemy.x = x;
                enemy.y = y;
                enemy.rank = rank;
                data.enemies.push_back(enemy);
                used[y][x] = true;
                placed = true;
            }
        }

        if ((int)data.enemies.size() != enemy_count ||
            !reachable_from_player(data, factory_cells))
        {
            continue;
        }

        int status_chance = 15 + index * 5;
        if (status_chance > 55)
        {
            status_chance = 55;
        }
        for (int y = 0; y < data.height; y++)
        {
            for (int x = 0; x < data.width; x++)
            {
                if (data.tiles[y][x] != '.' || factory_cells[y][x])
                {
                    continue;
                }
                if ((int)(rng() % 100) < status_chance)
                {
                    StatusSpawn spawn;
                    spawn.x = x;
                    spawn.y = y;
                    spawn.type = (StatusType)(rng() % 4);
                    spawn.turns = 2;
                    spawn.value = 5;
                    if (spawn.type == SHIELD)
                    {
                        spawn.value = 20;
                    }
                    data.statuses.push_back(spawn);
                }
                int item_roll = (int)(rng() % 100);
                int item_count = 0;
                if (item_roll < 5)
                {
                    item_count = 2;
                }
                else if (item_roll < 25)
                {
                    item_count = 1;
                }
                if (item_count > 0)
                {
                    ItemType first = (ItemType)(rng() % ITEM_COUNT);
                    ItemSpawn spawn;
                    spawn.x = x;
                    spawn.y = y;
                    spawn.type = first;
                    data.items.push_back(spawn);

                    if (item_count == 2)
                    {
                        ItemType second =
                            (ItemType)((first + 1 +
                                        rng() % (ITEM_COUNT - 1)) %
                                       ITEM_COUNT);
                        spawn.type = second;
                        data.items.push_back(spawn);
                    }
                }
            }
        }

        return data;
    }

    LevelData fallback;
    fallback.width = 25;
    fallback.height = 25;
    fallback.tiles.assign(fallback.height,
                          std::vector<char>(fallback.width, '.'));
    fallback.player.x = 1;
    fallback.player.y = 1;
    for (int i = 0; i < 3; i++)
    {
        EnemySpawn enemy;
        enemy.x = 20 - i * 3;
        enemy.y = 20;
        enemy.rank = 2 + index;
        fallback.enemies.push_back(enemy);
    }
    return fallback;
}

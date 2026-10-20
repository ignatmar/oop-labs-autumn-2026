#pragma once
#include "factory/factory.hpp"
#include "item/item.hpp"
#include "map/map.hpp"
#include "robot/robot.hpp"
#include <vector>

class Ability;
class Teleport_ab;

class World
{
  private:
    Map &map;
    Robot *player;
    std::vector<Robot *> enemies;
    std::vector<Factory *> factories;
    int steps;

    Ability *enemy_heal;
    Teleport_ab *teleport;

  public:
    World(Map &map);
    ~World();
    void set_player(Robot *robot);
    void add_enemy(Robot *robot);
    void add_factory(Factory *factory);

    std::vector<Factory *> &get_factories();

    Map &get_map();

    Robot *find_robot(int x, int y);
    int move_robot(Robot *robot, int new_x, int new_y, int damage, int action_points);
    int move_player(int dx, int dy, int action_points);

    void update_vision();
    void spawn_from_factories();
    void add_step();
    int get_steps();
    bool all_enemies_dead();
    bool player_alive();
    void player_update_statuses();
    void player_ready_slow();
    void enemies_ready_slow();
    void enemies_update_statuses();
    bool player_try_use_ability();
    bool player_is_slowed();
    void player_consume_slow();

    void refill_ap();
    bool is_player_turn_over();
    Robot &get_player_ref();
    std::vector<Robot *> &get_enemies_list();

    bool check_coords(size_t x, size_t y);
    Cell &get_cell(int x, int y);
    void apply_cell_statuses(Robot &robot, int x, int y);
    void apply_cell_items(Robot &robot, int x, int y);
    void player_use_item(ItemType type);
    void set_factory(size_t x, size_t y);
    void process_enemy_turns(double &timer);
    void start_enemy_turn();
    void enemy_use_ability(Robot *enemy);
    bool has_enemies_with_ap();
};

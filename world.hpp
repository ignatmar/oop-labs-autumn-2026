#pragma once
#include "factory.hpp"
#include "map.hpp"
#include "robot.hpp"
#include <vector>

class World
{
  private:
    Map &map;
    Robot *player;
    std::vector<Robot *> enemies;
    std::vector<Factory *> factories;
    int steps;

  public:
    World(Map &map);
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
    bool player_can_use_ability();

    void refill_ap();
    bool is_player_turn_over();
    Robot &get_player_ref();
    std::vector<Robot *> &get_enemies_list();
    int get_player_dmg();

    bool check_coords(size_t x, size_t y);
    Cell &get_cell(int x, int y);
    void set_factory(size_t x, size_t y);
    void process_enemy_turns(double &timer);
    bool has_enemies_with_ap();
};

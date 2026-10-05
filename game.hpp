#pragma once

#include "ability.hpp"
#include "map.hpp"
#include "render.hpp"
#include "robot.hpp"

enum GAME_STATUS
{
    GAME_RUNNING = 0,
    GAME_VICTORY = 1,
    GAME_DEFEAT = 2
};
class Game
{
  private:
    Map map;
    Robot &player_robot;
    int turn;
    double enemy_move_timer;
    GAME_STATUS game_over;
    Ability *around;
    Ability *range;
    Ability *heal;

  public:
    Game(Map &map, Robot &player_robot);
    void run(Render &render);
    int get_turn();
    void player_turn(Render &render);
    void enemy_turn(Robot *enemy);
    void end_turn();
    void update(Render &render);
    void kill_enemy(Robot *enemy);
};

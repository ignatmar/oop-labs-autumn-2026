#pragma once

#include "ability.hpp"
#include "render/render.hpp"
#include "world/world.hpp"

enum GAME_STATUS
{
    GAME_RUNNING = 0,
    GAME_VICTORY = 1,
    GAME_DEFEAT = 2
};
class Game
{
  private:
    World &world;
    int turn;
    double enemy_move_timer;
    GAME_STATUS game_over;
    int level_choice;

    Ability *around;
    Ability *range;
    Ability *heal;
    Ability *teleport;

    int level_up;

  public:
    Game(World &world);
    void run(Render &render);
    int get_turn();
    void player_turn(Render &render);
    void end_turn();
    void update(Render &render);

    void level_up_choice(int choice);
};

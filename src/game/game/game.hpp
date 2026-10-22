#pragma once

#include "ability.hpp"
#include "input/input.hpp"
#include "levels/level.hpp"
#include "render/render.hpp"
#include "save/save_game.hpp"
#include "view_modes/view_modes.hpp"
#include <vector>

enum GAME_STATUS
{
    GAME_RUNNING = 0,
    GAME_VICTORY = 1,
    GAME_DEFEAT = 2,
    GAME_LEVEL_COMPLETE = 3
};

enum class GameResult
{
    QUIT_TO_MENU,
    VICTORY,
    DEFEAT,
    WINDOW_CLOSED,
    ERROR
};

enum class LevelResult
{
    COMPLETE,
    QUIT_TO_MENU,
    VICTORY,
    DEFEAT,
    WINDOW_CLOSED
};

template <typename Levels> class Game
{
  private:
    Levels levels;
    PlayerProfile profile;
    Ability *abilities[4];
    Level *level;

    int current_level;
    int turn;
    double enemy_move_timer;
    GAME_STATUS state;
    int level_up;
    bool profile_ready;
    ViewModeState view_modes;
    Input input;

    std::vector<Ability *> ability_ptrs();
    bool apply_save_data(SaveData &save);
    void start_level();
    void player_turn(Render &render);
    void end_turn();
    void update(Render &render);
    void level_up_choice(int choice);
    LevelResult play_level(Render &render);

  public:
    Game(Levels levels);
    ~Game();

    GameResult run(Render &render, SaveData *save);
    SaveData make_save();
    int get_turn();
};

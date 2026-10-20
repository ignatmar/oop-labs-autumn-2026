#pragma once

#include "ability.hpp"
#include "around_ab/around_ab.hpp"
#include "heal_ab/heal_ab.hpp"
#include "input/input.hpp"
#include "levels/level.hpp"
#include "range_ab/range_ab.hpp"
#include "render/render.hpp"
#include "save/save_game.hpp"
#include "teleport_ab/teleport_ab.hpp"
#include "view_modes/view_modes.hpp"
#include <array>
#include <memory>
#include <utility>
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

template <typename Levels>
class Game
{
  private:
    Levels levels;
    PlayerProfile profile;
    std::array<std::unique_ptr<Ability>, 4> abilities;
    std::unique_ptr<Level> level;

    int current_level;
    int turn;
    double enemy_move_timer;
    GAME_STATUS state;
    int level_up;
    bool profile_ready;
    ViewModeState view_modes;

    void init_abilities()
    {
        abilities[0] = std::make_unique<Around_ab>(1, 10, 20);
        abilities[1] = std::make_unique<Range_ab>(3, 15, 25);
        abilities[2] = std::make_unique<Heal_ab>(0, 15, 20);
        abilities[3] = std::make_unique<Teleport_ab>(3, 0, 25);
    }

    std::vector<Ability *> ability_ptrs()
    {
        std::vector<Ability *> result;
        for (std::unique_ptr<Ability> &ability : abilities)
        {
            result.push_back(ability.get());
        }
        return result;
    }

    bool apply_save_data(SaveData &save)
    {
        if (levels.count() >= 0 && save.current_level >= levels.count())
        {
            return false;
        }
        current_level = save.current_level;
        profile = save.profile;
        profile_ready = true;

        for (int i = 0; i < 4; i++)
        {
            if (!save.abilities[i].unlocked)
            {
                continue;
            }
            abilities[i]->unlock();
            for (int j = 0; j < save.abilities[i].level; j++)
            {
                abilities[i]->upgrade();
            }
        }
        return true;
    }

    void start_level()
    {
        LevelData data = levels.make_level(current_level);
        if (!profile_ready)
        {
            profile = PlayerProfile::from_spawn(data.player);
            profile_ready = true;
        }

        level = std::make_unique<Level>(data, profile);
        turn = 1;
        enemy_move_timer = 0;
        state = GAME_RUNNING;
        level_up = 0;
    }

    void player_turn(Render &render)
    {
        World &world = level->get_world();

        int item = Input::get_item();
        if (item >= 0)
        {
            world.player_use_item((ItemType)item);
        }

        Input::AbilityCmd cmd =
            Input::get_ability(render.get_draw_scale(), render.get_offset_x());
        if (cmd.used && world.player_try_use_ability())
        {
            if (cmd.slot == 1)
            {
                abilities[0]->use(world.get_player_ref(), world, 0, 0);
            }
            else if (cmd.slot == 2)
            {
                abilities[1]->use(world.get_player_ref(), world,
                                 (size_t)cmd.target_x, (size_t)cmd.target_y);
            }
            else if (cmd.slot == 3)
            {
                abilities[2]->use(world.get_player_ref(), world, 0, 0);
            }
            else if (cmd.slot == 4)
            {
                abilities[3]->use(world.get_player_ref(), world,
                                 (size_t)cmd.target_x, (size_t)cmd.target_y);
            }
        }

        if (world.is_player_turn_over())
        {
            end_turn();
        }
    }

    void end_turn()
    {
        World &world = level->get_world();
        world.player_update_statuses();
        world.player_ready_slow();
        turn = 0;
        world.add_step();
        world.spawn_from_factories();
        world.start_enemy_turn();
    }

    void update(Render &render)
    {
        World &world = level->get_world();
        world.update_vision();
        if (!world.player_alive())
        {
            state = GAME_DEFEAT;
            return;
        }
        if (level_up > 0)
        {
            return;
        }
        if (turn == 1)
        {
            if (world.player_is_slowed())
            {
                world.player_consume_slow();
                end_turn();
                return;
            }
            player_turn(render);
        }
        else
        {
            if (GetTime() - enemy_move_timer >= 0.1)
            {
                world.process_enemy_turns(enemy_move_timer);
                enemy_move_timer = GetTime();
                if (!world.has_enemies_with_ap())
                {
                    world.enemies_update_statuses();
                    world.enemies_ready_slow();
                    world.refill_ap();
                    turn = 1;
                }
            }
        }
    }

    void level_up_choice(int choice)
    {
        if (choice == 0)
        {
            return;
        }

        int index = choice - 1;
        if (index < 0 || index >= 4)
        {
            return;
        }

        if (abilities[index]->is_unlocked())
        {
            abilities[index]->upgrade();
        }
        else
        {
            abilities[index]->unlock();
        }

        level_up--;
    }

    LevelResult play_level(Render &render)
    {
        World &world = level->get_world();

        while (!WindowShouldClose())
        {
            if (Input::fullscreen_requested())
            {
                render.toggle_fullscreen();
            }

            if (Input::menu_requested())
            {
                return LevelResult::QUIT_TO_MENU;
            }

            if (state == GAME_RUNNING)
            {
                if (turn == 1 && level_up == 0)
                {
                    Input::MoveCmd move = Input::get_move();
                    if (move.moved)
                    {
                        int gained = world.move_player(move.dx, move.dy, 3);
                        if (gained > 0)
                        {
                            level_up += gained;
                        }
                    }
                }

                update(render);

                if (state == GAME_RUNNING && level_up == 0 &&
                    world.all_enemies_dead())
                {
                    if (levels.has_next(current_level))
                    {
                        state = GAME_LEVEL_COMPLETE;
                    }
                    else
                    {
                        state = GAME_VICTORY;
                    }
                }
            }

            int view_mode = Input::get_view_mode();
            if (view_mode >= 0)
            {
                toggle_view_mode(view_modes, view_mode);
            }

            render.draw(world, state, level_up, ability_ptrs(), view_modes);

            if (level_up > 0)
            {
                int choice = Input::get_level_up_choice();
                level_up_choice(choice);
            }

            if (state == GAME_LEVEL_COMPLETE && level_up == 0 &&
                Input::confirm_requested())
            {
                return LevelResult::COMPLETE;
            }
            if (state == GAME_VICTORY && Input::confirm_requested())
            {
                return LevelResult::VICTORY;
            }
            if (state == GAME_DEFEAT && Input::confirm_requested())
            {
                return LevelResult::DEFEAT;
            }
        }

        return LevelResult::WINDOW_CLOSED;
    }

  public:
    Game(Levels levels)
        : levels(std::move(levels)), current_level(0), turn(1),
          enemy_move_timer(0), state(GAME_RUNNING), level_up(0),
          profile_ready(false)
    {
        init_abilities();
    }

    GameResult run(Render &render, SaveData *save)
    {
        if (save != nullptr)
        {
            if (!apply_save_data(*save))
            {
                return GameResult::ERROR;
            }
        }
        else
        {
            current_level = 0;
            profile_ready = false;
        }

        if (levels.count() == 0)
        {
            return GameResult::ERROR;
        }

        while (!WindowShouldClose())
        {
            if (!level)
            {
                start_level();
            }

            LevelResult result = play_level(render);
            if (result == LevelResult::COMPLETE)
            {
                profile = level->make_profile();
                if (!levels.has_next(current_level))
                {
                    return GameResult::VICTORY;
                }
                current_level++;
                level.reset();
                continue;
            }
            if (result == LevelResult::QUIT_TO_MENU)
            {
                return GameResult::QUIT_TO_MENU;
            }
            if (result == LevelResult::VICTORY)
            {
                return GameResult::VICTORY;
            }
            if (result == LevelResult::DEFEAT)
            {
                return GameResult::DEFEAT;
            }
            return GameResult::WINDOW_CLOSED;
        }

        return GameResult::WINDOW_CLOSED;
    }

    SaveData make_save()
    {
        SaveData save;
        save.mode = levels.mode();
        save.current_level = current_level;
        save.seed = levels.get_seed();
        save.profile = profile;
        for (int i = 0; i < 4; i++)
        {
            save.abilities[i].unlocked = abilities[i]->is_unlocked();
            save.abilities[i].level = abilities[i]->get_level();
        }
        return save;
    }

    int get_turn() { return turn; }
};

#include "game/game.hpp"
#include "around_ab/around_ab.hpp"
#include "heal_ab/heal_ab.hpp"
#include "levels/fixed_levels/fixed_levels.hpp"
#include "levels/random_levels/random_levels.hpp"
#include "range_ab/range_ab.hpp"
#include "teleport_ab/teleport_ab.hpp"
#include <raylib.h>

template <typename Levels>
std::vector<Ability *> Game<Levels>::ability_ptrs()
{
    std::vector<Ability *> result;
    for (int i = 0; i < 4; i++)
    {
        result.push_back(abilities[i]);
    }
    return result;
}

template <typename Levels>
bool Game<Levels>::apply_save_data(SaveData &save)
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

template <typename Levels>
void Game<Levels>::start_level()
{
    LevelData data = levels.make_level(current_level);
    if (!profile_ready)
    {
        profile.from_spawn(data.player);
        profile_ready = true;
    }

    level = new Level(data, profile);
    turn = 1;
    enemy_move_timer = 0;
    state = GAME_RUNNING;
    level_up = 0;
}

template <typename Levels>
void Game<Levels>::player_turn(Render &render)
{
    World &world = level->get_world();

    int item = input.get_item();
    if (item >= 0)
    {
        world.player_use_item((ItemType)item);
    }

    Input::AbilityCmd cmd =
        input.get_ability(render.get_draw_scale(), render.get_offset_x());
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

template <typename Levels>
void Game<Levels>::end_turn()
{
    World &world = level->get_world();
    world.player_update_statuses();
    world.player_ready_slow();
    turn = 0;
    world.add_step();
    world.spawn_from_factories();
    world.start_enemy_turn();
}

template <typename Levels>
void Game<Levels>::update(Render &render)
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

template <typename Levels>
void Game<Levels>::level_up_choice(int choice)
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

template <typename Levels>
LevelResult Game<Levels>::play_level(Render &render)
{
    World &world = level->get_world();

    while (!WindowShouldClose())
    {
        if (input.fullscreen_requested())
        {
            render.toggle_fullscreen();
        }

        if (input.menu_requested())
        {
            return LevelResult::QUIT_TO_MENU;
        }

        if (state == GAME_RUNNING)
        {
            if (turn == 1 && level_up == 0)
            {
                Input::MoveCmd move = input.get_move();
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

        int view_mode = input.get_view_mode();
        if (view_mode >= 0)
        {
            toggle_view_mode(view_modes, view_mode);
        }

        render.draw(world, state, level_up, ability_ptrs(), view_modes);

        if (level_up > 0)
        {
            int choice = input.get_level_up_choice();
            level_up_choice(choice);
        }

        if (state == GAME_LEVEL_COMPLETE && level_up == 0 &&
            input.confirm_requested())
        {
            return LevelResult::COMPLETE;
        }
        if (state == GAME_VICTORY && input.confirm_requested())
        {
            return LevelResult::VICTORY;
        }
        if (state == GAME_DEFEAT && input.confirm_requested())
        {
            return LevelResult::DEFEAT;
        }
    }

    return LevelResult::WINDOW_CLOSED;
}

template <typename Levels>
Game<Levels>::Game(Levels levels)
    : levels(levels), profile(),
      abilities{new Around_ab(1, 10, 20), new Range_ab(3, 15, 25),
                new Heal_ab(0, 15, 20), new Teleport_ab(3, 0, 25)},
      level(nullptr), current_level(0), turn(1), enemy_move_timer(0),
      state(GAME_RUNNING), level_up(0), profile_ready(false)
{
}

template <typename Levels>
Game<Levels>::~Game()
{
    delete level;
    for (int i = 0; i < 4; i++)
    {
        delete abilities[i];
    }
}

template <typename Levels>
GameResult Game<Levels>::run(Render &render, SaveData *save)
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
            delete level;
            level = nullptr;
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

template <typename Levels>
SaveData Game<Levels>::make_save()
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

template <typename Levels>
int Game<Levels>::get_turn()
{
    return turn;
}

template class Game<FixedLevels>;
template class Game<RandomLevels<RANDOM_LEVEL_COUNT>>;
template class Game<EndlessLevels>;

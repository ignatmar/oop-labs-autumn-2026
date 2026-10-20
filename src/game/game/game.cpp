#include "game/game.hpp"
#include "ability.hpp"
#include "around_ab/around_ab.hpp"
#include "heal_ab/heal_ab.hpp"
#include "input/input.hpp"
#include "range_ab/range_ab.hpp"
#include "teleport_ab/teleport_ab.hpp"
#include <raylib.h>

Game::Game(World &world)
    : world(world), turn(1), enemy_move_timer(0), game_over(GAME_RUNNING),
      level_up(0)
{
    around = new Around_ab(1, 10, 20);
    range = new Range_ab(3, 15, 25);
    heal = new Heal_ab(0, 15, 20);
    teleport = new Teleport_ab(3, 0, 25);
}

void Game::player_turn(Render &render)
{
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
            around->use(world.get_player_ref(), world, 0, 0);
        }
        else if (cmd.slot == 2)
        {
            range->use(world.get_player_ref(), world, (size_t)cmd.target_x,
                       (size_t)cmd.target_y);
        }
        else if (cmd.slot == 3)
        {
            heal->use(world.get_player_ref(), world, 0, 0);
        }
        else if (cmd.slot == 4)
        {
            teleport->use(world.get_player_ref(), world, (size_t)cmd.target_x,
                          (size_t)cmd.target_y);
        }
    }

    if (world.is_player_turn_over())
    {
        end_turn();
    }
}

void Game::end_turn()
{
    world.player_update_statuses();
    world.player_ready_slow();
    turn = 0;
    world.add_step();
    world.spawn_from_factories();
    world.start_enemy_turn();
}

void Game::update(Render &render)
{
    world.update_vision();
    if (!world.player_alive())
    {
        game_over = GAME_DEFEAT;
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

int Game::get_turn() { return turn; }

void Game::run(Render &render)
{
    render.start(world.get_map());

    while (!WindowShouldClose())
    {
        if (!game_over)
        {

            if (turn == 1)
            {
                Input::MoveCmd mv = Input::get_move();
                if (mv.moved)
                {
                    int tmp = world.move_player(mv.dx, mv.dy, 3);
                    if (tmp > 0)
                    {
                        level_up += tmp;
                    }
                }
            }
            update(render);
            if (world.all_enemies_dead())
            {
                game_over = GAME_VICTORY;
            }
        }
        std::vector<Ability *> abilities;
        abilities.push_back(around);
        abilities.push_back(range);
        abilities.push_back(heal);
        abilities.push_back(teleport);

        int view_mode = Input::get_view_mode();
        if (view_mode >= 0)
        {
            toggle_view_mode(view_modes, view_mode);
        }

        render.draw(world, game_over, level_up, abilities, view_modes);
        if (level_up > 0)
        {
            int level_choice = Input::get_level_up_choice();
            level_up_choice(level_choice);
        }
    }

    CloseWindow();
}

void Game::level_up_choice(int choice)
{
    if (choice == 0)
    {
        return;
    }

    if (choice == 1)
    {
        if (around->is_unlocked())
        {
            around->upgrade();
        }
        else
        {
            around->unlock();
        }
    }
    else if (choice == 2)
    {
        if (range->is_unlocked())
        {
            range->upgrade();
        }
        else
        {
            range->unlock();
        }
    }
    else if (choice == 3)
    {
        if (heal->is_unlocked())
        {
            heal->upgrade();
        }
        else
        {
            heal->unlock();
        }
    }
    else if (choice == 4)
    {
        if (teleport->is_unlocked())
        {
            teleport->upgrade();
        }
        else
        {
            teleport->unlock();
        }
    }

    level_up--;
}

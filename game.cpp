#include "game.hpp"
#include "ability.hpp"
#include "around_ab.hpp"
#include "factory.hpp"
#include "heal_ab.hpp"
#include "input.hpp"
#include "range_ab.hpp"
#include "teleport_ab.hpp"
#include <raylib.h>

Game::Game(World &world)
    : world(world), turn(1), enemy_move_timer(0), game_over(GAME_RUNNING),
      level_up(0)
{
    around = new Around_ab(1, 10, 20);
    range = new Range_ab(3, 15, 25);
    heal = new Heal_ab(0, 15, 20);
    teleport = new Teleport_ab(5, 0, 25);
}

void Game::player_turn(Render &render)
{
    if (!world.player_can_use_ability())
    {
        world.player_update_statuses();
        if (world.is_player_turn_over())
        {
            end_turn();
        }
        return;
    }

    Input::AbilityCmd cmd = Input::get_ability(render.get_draw_scale());
    if (!cmd.used)
    {
        world.player_update_statuses();
        if (world.is_player_turn_over())
        {
            end_turn();
        }
        return;
    }


    if (cmd.slot == 1)
    {
        around->use(world.get_player_ref(), world, 0, 0);
    }
    else if (cmd.slot == 2)
    {
        range->use(world.get_player_ref(), world, (size_t)cmd.target_x, (size_t)cmd.target_y);
    }
    else if (cmd.slot == 3)
    {
        heal->use(world.get_player_ref(), world, 0, 0);
    }
    else if (cmd.slot == 4)
    {
        teleport->use(world.get_player_ref(), world, (size_t)cmd.target_x, (size_t)cmd.target_y);
    }

    world.player_update_statuses();
    if (world.is_player_turn_over())
    {
        end_turn();
    }
}



void Game::end_turn()
{
    turn = 0;
    world.add_step();
    world.spawn_from_factories();
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
        player_turn(render);
    }

    else
    {
        if (GetTime() - enemy_move_timer >= 0.01)
        {
            world.process_enemy_turns(enemy_move_timer);
            enemy_move_timer = GetTime();
            if (!world.has_enemies_with_ap())
            {
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
        if (world.player_alive())
        {
            render.draw(world.get_map(), world.get_player_ref(), world.get_enemies_list(), game_over, level_up, abilities);
        }
        else
        {
            Robot dummy(0, 0, 0, 0, 0, 0, 0, false);
            render.draw(world.get_map(), dummy, world.get_enemies_list(), game_over, level_up, abilities);
        }
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

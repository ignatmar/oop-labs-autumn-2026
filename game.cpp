#include "game.hpp"
#include "ability.hpp"
#include "around_ab.hpp"
#include "factory.hpp"
#include "heal_ab.hpp"
#include "range_ab.hpp"
#include <raylib.h>

Game::Game(Map &map, Robot &player_robot)
    : map(map), player_robot(player_robot), turn(1), enemy_move_timer(0),
      game_over(GAME_RUNNING)
{
    around = new Around_ab(1, 10, 20);
    range = new Range_ab(3, 20, 25);
    heal = new Heal_ab(0, 15, 20);
}

void Game::player_turn(Render &render)
{
    if (IsKeyPressed(KEY_ONE))
    {
        around->use(player_robot, map, 0, 0);
    }

    if (IsKeyPressed(KEY_TWO))
    {
        Vector2 mouse = GetMousePosition();

        int x = mouse.x / render.get_draw_scale();
        int y = mouse.y / render.get_draw_scale();

        range->use(player_robot, map, x, y);
    }

    if (IsKeyPressed(KEY_THREE))
    {
        heal->use(player_robot, map, 0, 0);
    }

    if (player_robot.get_ap() <= 0)
    {
        end_turn();
    }
}

void Game::enemy_turn(Robot *enemy)
{
    int e_x = enemy->get_x();
    int e_y = enemy->get_y();
    int p_x = player_robot.get_x();
    int p_y = player_robot.get_y();

    if (e_x < p_x)
    {
        map.move_robot(enemy, e_x + 1, e_y);
    }
    else if (e_x > p_x)
    {
        map.move_robot(enemy, e_x - 1, e_y);
    }
    else if (e_y < p_y)
    {
        map.move_robot(enemy, e_x, e_y + 1);
    }
    else if (e_y > p_y)
    {
        map.move_robot(enemy, e_x, e_y - 1);
    }
}

void Game::end_turn()
{
    turn = 0;
    map.add_step();
    for (Factory *factory : map.get_factories())
    {
        factory->spawn(map);
    }
}

void Game::update(Render &render)
{
    map.update_vision(player_robot.get_vision(), player_robot.get_x(),
                      player_robot.get_y());

    if (player_robot.get_hp() <= 0)
    {
        game_over = GAME_DEFEAT;
        return;
    }

    if (turn == 1)
    {
        player_turn(render);
    }
    else
    {
        if (GetTime() - enemy_move_timer >= 0.05)
        {
            for (Robot *enemy : map.get_enemy_robots())
            {
                if (enemy->get_ap() > 0 && enemy->get_hp() > 0)
                {
                    enemy_turn(enemy);
                    enemy_move_timer = GetTime();
                    return;
                }
            }

            for (Robot *enemy : map.get_enemy_robots())
            {
                enemy->set_ap(enemy->get_max_ap());
            }

            player_robot.set_ap(player_robot.get_max_ap());
            turn = 1;
        }
    }
}

int Game::get_turn() { return turn; }

void Game::run(Render &render)
{
    render.start(map);

    while (!WindowShouldClose())
    {
        if (!game_over)
        {

            if (turn == 1)
            {
                if (IsKeyPressed(KEY_W))
                    map.move_robot(&player_robot, player_robot.get_x(),
                                   player_robot.get_y() - 1);

                else if (IsKeyPressed(KEY_S))
                    map.move_robot(&player_robot, player_robot.get_x(),
                                   player_robot.get_y() + 1);

                else if (IsKeyPressed(KEY_A))
                    map.move_robot(&player_robot, player_robot.get_x() - 1,
                                   player_robot.get_y());

                else if (IsKeyPressed(KEY_D))
                    map.move_robot(&player_robot, player_robot.get_x() + 1,
                                   player_robot.get_y());
            }

            update(render);

            if (player_robot.get_kills() == map.get_enemy_robots().size())
            {
                game_over = GAME_VICTORY;
            }
        }
        render.draw(map, player_robot, map.get_enemy_robots(), game_over);
    }

    CloseWindow();
}

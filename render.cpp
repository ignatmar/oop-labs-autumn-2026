#include "render.hpp"
#include "cell.hpp"
#include "robot.hpp"
#include <cstdlib>
#include <string>

Render::Render(int draw_scale) { this->draw_scale = draw_scale; }

void Render::draw(Map &map, Robot &player,
                  const std::vector<Robot *> enemy_robots, int game_over)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int y = 0; y < map.get_height(); y++)
    {
        for (int x = 0; x < map.get_width(); x++)
        {
            Cell &cell = map.get_cell(x, y);
            Color cell_color;
            switch (cell.get_surface())
            {
            case FACTORY:
                cell_color = PURPLE;
                break;
            case MOUNTAIN:
                cell_color = DARKGRAY;
                break;
            case GRASS:
                cell_color = GREEN;
                break;
            case SAND:
                cell_color = YELLOW;
                break;
            default:
                cell_color = WHITE;
                break;
            }

            int p_x = player.get_x();
            int p_y = player.get_y();
            int rad = player.get_vision();

            if (std::abs(x - p_x) + std::abs(y - p_y) < rad ||
                map.get_cell(x, y).if_visible())
            {
                DrawRectangle(x * draw_scale, y * draw_scale, draw_scale,
                              draw_scale, cell_color);
            }
            else
            {
                DrawRectangle(x * draw_scale, y * draw_scale, draw_scale,
                              draw_scale, BLACK);
            }

            DrawRectangleLines(x * draw_scale, y * draw_scale, draw_scale,
                               draw_scale, GRAY);
            if (cell.get_robot() != nullptr)
            {
                if (cell.get_robot()->get_team() == 1)
                {
                    DrawCircle(x * draw_scale + draw_scale / 2,
                               y * draw_scale + draw_scale / 2,
                               draw_scale / 3.0, BLUE);
                }
                else if (std::abs(x - p_x) + std::abs(y - p_y) < rad ||
                         cell.if_visible())
                {
                    DrawCircle(x * draw_scale + draw_scale / 2,
                               y * draw_scale + draw_scale / 2,
                               draw_scale / 3.0, RED);
                }
            }
        }
    }

    draw_ap_text(map, player, enemy_robots);
    if (game_over)
    {
        draw_game_over(game_over);
    }
    EndDrawing();
}

void Render::start(Map &map)
{
    InitWindow(map.get_width() * draw_scale + 200,
               map.get_height() * draw_scale, "Game");

    SetTargetFPS(60);
}

void Render::draw_ap_text(Map &map, Robot &player,
                          std::vector<Robot *> enemy_robots)
{
    int panel_x = map.get_width() * draw_scale;
    int panel_width = 200;

    DrawRectangle(panel_x, 0, panel_width, map.get_height() * draw_scale,
                  BLACK);

    int name_y = 20;

    DrawText("PLAYER", panel_x + 20, name_y, 20, WHITE);

    float hp_percent = (float)player.get_hp() / player.get_max_hp();
    float ap_percent = (float)player.get_ap() / player.get_max_ap();

    std::string player_ap = "AP: " + std::to_string(player.get_ap()) + "/" +
                            std::to_string(player.get_max_ap());

    DrawText(player_ap.c_str(), panel_x + 20, name_y + 30, 20, WHITE);

    DrawRectangle(panel_x + 20, name_y + 55, 160, 15, DARKGRAY);
    DrawRectangle(panel_x + 20, name_y + 55, 160 * ap_percent, 15, BLUE);

    std::string player_hp = "HP: " + std::to_string(player.get_hp()) + "/" +
                            std::to_string(player.get_max_hp());

    DrawText(player_hp.c_str(), panel_x + 20, name_y + 80, 20, WHITE);

    DrawRectangle(panel_x + 20, name_y + 105, 160, 15, DARKGRAY);
    DrawRectangle(panel_x + 20, name_y + 105, 160 * hp_percent, 15, RED);

    int y = 200;

    for (size_t i = 0; i < enemy_robots.size(); i++)
    {
        std::string name = "ENEMY " + std::to_string(i + 1);

        float hp_percent =
            (float)enemy_robots[i]->get_hp() / enemy_robots[i]->get_max_hp();

        float ap_percent =
            (float)enemy_robots[i]->get_ap() / enemy_robots[i]->get_max_ap();

        DrawText(name.c_str(), panel_x + 20, y, 20, WHITE);

        std::string ap = "AP: " + std::to_string(enemy_robots[i]->get_ap()) +
                         "/" + std::to_string(enemy_robots[i]->get_max_ap());

        DrawText(ap.c_str(), panel_x + 20, y + 30, 20, WHITE);

        DrawRectangle(panel_x + 20, y + 55, 160, 15, DARKGRAY);
        DrawRectangle(panel_x + 20, y + 55, 160 * ap_percent, 15, BLUE);

        std::string hp = "HP: " + std::to_string(enemy_robots[i]->get_hp()) +
                         "/" + std::to_string(enemy_robots[i]->get_max_hp());

        DrawText(hp.c_str(), panel_x + 20, y + 80, 20, WHITE);

        DrawRectangle(panel_x + 20, y + 105, 160, 15, DARKGRAY);
        DrawRectangle(panel_x + 20, y + 105, 160 * hp_percent, 15, RED);

        y += 150;
    }
}

void Render::draw_game_over(int game_over)
{
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    DrawRectangle(0, 0, width, height, Fade(BLACK, 0.7f));

    if (game_over == 1)
    {
        DrawText("VICTORY", width / 2 - 200, height / 2 - 60, 80, LIME);
    }
    else
    {
        DrawText("DEFEAT", width / 2 - 200, height / 2 - 60, 80, RED);
    }
}

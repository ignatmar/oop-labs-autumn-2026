#include "render.hpp"
#include "cell.hpp"
#include "robot.hpp"
#include <cstdlib>
#include <raylib.h>
#include <string>

Render::Render(int draw_scale) { this->draw_scale = draw_scale; }

void Render::draw(Map &map, Robot &player,
                  const std::vector<Robot *> enemy_robots, int game_over,
                  int level_up, std::vector<Ability *> abilities)
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
                    // DrawCircle(x * draw_scale + draw_scale / 2,
                    //           y * draw_scale + draw_scale / 2,
                    //           draw_scale / 3.0, BLUE);

                    DrawTexturePro(player_texture,
                                   {0, 0, (float)player_texture.width,
                                    (float)player_texture.height},
                                   {(float)(x * draw_scale),
                                    (float)(y * draw_scale), (float)draw_scale,
                                    (float)draw_scale},
                                   {0, 0}, 0, WHITE);
                }
                else if (std::abs(x - p_x) + std::abs(y - p_y) < rad ||
                         cell.if_visible())
                {
                    // DrawCircle(x * draw_scale + draw_scale / 2,
                    //          y * draw_scale + draw_scale / 2,
                    //           draw_scale / 3.0, RED);
                    DrawTexturePro(enemy_texture,
                                   {0, 0, (float)player_texture.width,
                                    (float)player_texture.height},
                                   {(float)(x * draw_scale),
                                    (float)(y * draw_scale), (float)draw_scale,
                                    (float)draw_scale},
                                   {0, 0}, 0, WHITE);
                }
            }
        }
    }

    draw_ap_text(map, player, enemy_robots);
    draw_abilities(map, abilities);
    if (game_over)
    {
        draw_game_over(game_over);
    }
    if (level_up && !game_over)
    {
        draw_level_up();
    }
    EndDrawing();
}

void Render::start(Map &map)
{
    InitWindow(map.get_width() * draw_scale + 200,
               map.get_height() * draw_scale + 120, "Game");

    around_texture = LoadTexture("assets/ab/around.png");
    range_texture = LoadTexture("assets/ab/range.png");
    heal_texture = LoadTexture("assets/ab/heal.png");
    teleport_texture = LoadTexture("assets/ab/teleport.png");

    player_texture = LoadTexture("assets/robots/player.png");
    enemy_texture = LoadTexture("assets/robots/enemy.png");

    grass_texture = LoadTexture("assets/bg/grass.png");

    SetTargetFPS(60);
}

void Render::draw_abilities(Map &map, std::vector<Ability *> abilities)
{
    int panel_y = map.get_height() * draw_scale;
    int panel_height = 120;

    int square_size = 70;
    int spacing = 35;

    DrawRectangle(0, panel_y, GetScreenWidth(), panel_height, BLACK);

    int total_width = square_size * 4 + spacing * 3;
    int start_x = (GetScreenWidth() - total_width) / 2;

    const char *names[] = {"Around hit", "Range hit", "Heal", "Teleport"};
    Texture2D textures[] = {around_texture, range_texture, heal_texture,
                            teleport_texture};
    for (int i = 0; i < 4; i++)
    {
        int x = start_x + i * (square_size + spacing);

        DrawRectangleLines(x, panel_y + 10, square_size, square_size, WHITE);
        if (abilities[i]->is_unlocked())
        {
            DrawTexturePro(
                textures[i],
                {0, 0, (float)textures[i].width, (float)textures[i].height},
                {(float)x, (float)panel_y + 10, 70, 70}, {0, 0}, 0, WHITE);
        }

        int text_width = MeasureText(names[i], 16);
        int text_x = x + (square_size - text_width) / 2;

        DrawText(names[i], text_x, panel_y + 90, 16, WHITE);
    }
}

void Render::draw_ap_text(Map &map, Robot &player,
                          std::vector<Robot *> enemy_robots)
{
    int panel_x = map.get_width() * draw_scale;
    int panel_width = 200;

    DrawRectangle(panel_x, 0, panel_width, map.get_height() * draw_scale,
                  BLACK);

    int name_y = 20;

    std::string player_text = "PLAYER LVL " + std::to_string(player.get_lvl());
    DrawText(player_text.c_str(), panel_x + 20, name_y, 20, WHITE);

    float hp_percent = (float)player.get_hp() / player.get_max_hp();
    float ap_percent = (float)player.get_ap() / player.get_max_ap();
    float mana_percent = (float)player.get_mana() / player.get_max_mana();

    std::string player_ap = "AP: " + std::to_string(player.get_ap()) + "/" +
                            std::to_string(player.get_max_ap());

    DrawText(player_ap.c_str(), panel_x + 20, name_y + 30, 20, WHITE);

    DrawRectangle(panel_x + 20, name_y + 55, 160, 15, DARKGRAY);
    DrawRectangle(panel_x + 20, name_y + 55, 160 * ap_percent, 15, YELLOW);

    std::string player_hp = "HP: " + std::to_string(player.get_hp()) + "/" +
                            std::to_string(player.get_max_hp());

    DrawText(player_hp.c_str(), panel_x + 20, name_y + 80, 20, WHITE);

    DrawRectangle(panel_x + 20, name_y + 105, 160, 15, DARKGRAY);
    DrawRectangle(panel_x + 20, name_y + 105, 160 * hp_percent, 15, RED);

    std::string player_mana = "MANA: " + std::to_string(player.get_mana()) +
                              "/" + std::to_string(player.get_max_mana());

    DrawText(player_mana.c_str(), panel_x + 20, name_y + 130, 20, WHITE);

    DrawRectangle(panel_x + 20, name_y + 155, 160, 15, DARKGRAY);
    DrawRectangle(panel_x + 20, name_y + 155, 160 * mana_percent, 15, BLUE);

    std::string player_shield =
        "SHIELD: " + std::to_string(player.get_shield());

    DrawText(player_shield.c_str(), panel_x + 20, name_y + 180, 20, WHITE);

    DrawRectangle(panel_x + 20, name_y + 205, 160, 15, DARKGRAY);
    DrawRectangle(panel_x + 20, name_y + 205, 160, 15, WHITE);

    int y = 290;

    for (size_t i = 0; i < enemy_robots.size(); i++)
    {
        if (enemy_robots[i]->get_hp() > 0)
        {
            std::string name = "ENEMY " + std::to_string(i + 1);

            float hp_percent = (float)enemy_robots[i]->get_hp() /
                               enemy_robots[i]->get_max_hp();

            float ap_percent = (float)enemy_robots[i]->get_ap() /
                               enemy_robots[i]->get_max_ap();
            float mana_percent = (float)enemy_robots[i]->get_mana() /
                                 enemy_robots[i]->get_max_mana();

            DrawText(name.c_str(), panel_x + 20, y, 20, WHITE);

            std::string ap =
                "AP: " + std::to_string(enemy_robots[i]->get_ap()) + "/" +
                std::to_string(enemy_robots[i]->get_max_ap());

            DrawText(ap.c_str(), panel_x + 20, y + 30, 20, WHITE);

            DrawRectangle(panel_x + 20, y + 55, 160, 15, DARKGRAY);
            DrawRectangle(panel_x + 20, y + 55, 160 * ap_percent, 15, YELLOW);

            std::string hp =
                "HP: " + std::to_string(enemy_robots[i]->get_hp()) + "/" +
                std::to_string(enemy_robots[i]->get_max_hp());

            DrawText(hp.c_str(), panel_x + 20, y + 80, 20, WHITE);

            DrawRectangle(panel_x + 20, y + 105, 160, 15, DARKGRAY);
            DrawRectangle(panel_x + 20, y + 105, 160 * hp_percent, 15, RED);

            std::string mana =
                "MANA: " + std::to_string(enemy_robots[i]->get_mana()) + "/" +
                std::to_string(enemy_robots[i]->get_max_mana());

            DrawText(mana.c_str(), panel_x + 20, y + 130, 20, WHITE);

            DrawRectangle(panel_x + 20, y + 155, 160, 15, DARKGRAY);
            DrawRectangle(panel_x + 20, y + 155, 160 * mana_percent, 15, BLUE);

            y += 200;
        }
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

int Render::get_draw_scale() { return draw_scale; }

void Render::draw_level_up()
{
    int width = 700;
    int height = 700;

    int start_x = (GetScreenWidth() - width) / 2;
    int start_y = (GetScreenHeight() - height) / 2;

    DrawRectangle(start_x, start_y, width, height, DARKGRAY);
    DrawRectangleLines(start_x, start_y, width, height, WHITE);

    DrawText("LEVEL UP!", start_x + 250, start_y + 30, 30, WHITE);

    int square_size = 250;
    int spacing = 30;

    int x1 = start_x + 70;
    int x2 = x1 + square_size + spacing;
    int y1 = start_y + 100;
    int y2 = y1 + square_size + spacing;

    DrawRectangleLines(x1, y1, square_size, square_size, WHITE);
    DrawRectangleLines(x2, y1, square_size, square_size, WHITE);
    DrawRectangleLines(x1, y2, square_size, square_size, WHITE);
    DrawRectangleLines(x2, y2, square_size, square_size, WHITE);

    DrawText("Around", x1 + 80, y1 + 110, 25, WHITE);
    DrawText("Range", x2 + 90, y1 + 110, 25, WHITE);
    DrawText("Heal", x1 + 95, y2 + 110, 25, WHITE);
    DrawText("Teleport", x2 + 70, y2 + 110, 25, WHITE);
}

int Render::get_level_up_choice()
{
    int width = 700;
    int height = 700;

    int start_x = (GetScreenWidth() - width) / 2;
    int start_y = (GetScreenHeight() - height) / 2;

    int square_size = 250;
    int spacing = 30;

    int x1 = start_x + 70;
    int x2 = x1 + square_size + spacing;
    int y1 = start_y + 100;
    int y2 = y1 + square_size + spacing;

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return 0;
    }

    Vector2 mouse = GetMousePosition();

    if (mouse.x >= x1 && mouse.x <= x1 + square_size && mouse.y >= y1 &&
        mouse.y <= y1 + square_size)
    {
        return 1;
    }

    if (mouse.x >= x2 && mouse.x <= x2 + square_size && mouse.y >= y1 &&
        mouse.y <= y1 + square_size)
    {
        return 2;
    }

    if (mouse.x >= x1 && mouse.x <= x1 + square_size && mouse.y >= y2 &&
        mouse.y <= y2 + square_size)
    {
        return 3;
    }

    if (mouse.x >= x2 && mouse.x <= x2 + square_size && mouse.y >= y2 &&
        mouse.y <= y2 + square_size)
    {
        return 4;
    }

    return 0;
}

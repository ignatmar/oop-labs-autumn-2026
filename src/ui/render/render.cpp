#include "render/render.hpp"
#include "cell/cell.hpp"
#include "robot/robot.hpp"
#include <cstdlib>
#include <raylib.h>
#include <string>

Render::Render(int draw_scale) : draw_scale(draw_scale), offset_x(0) {}

void Render::draw(World &world, int game_over, int level_up,
                  std::vector<Ability *> abilities, ViewModeState &view_modes)
{
    Map &map = world.get_map();
    Robot &player = world.get_player_ref();
    std::vector<Robot *> &enemies = world.get_enemies_list();

    int p_x = player.get_x();
    int p_y = player.get_y();
    int rad = player.get_vision();

    int map_w = map.get_width() * draw_scale;
    int panel_width = 200;
    offset_x = (GetScreenWidth() - map_w) / 2;
    int max_offset = GetScreenWidth() - panel_width - map_w;
    if (offset_x > max_offset)
    {
        offset_x = max_offset;
    }
    if (offset_x < 0)
    {
        offset_x = 0;
    }

    BeginDrawing();
    ClearBackground(BLACK);
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

            if (std::abs(x - p_x) + std::abs(y - p_y) < rad ||
                map.get_cell(x, y).if_visible())
            {
                DrawRectangle(offset_x + x * draw_scale, y * draw_scale,
                              draw_scale, draw_scale, cell_color);
                draw_cell_highlights(cell, x, y, view_modes);
                draw_cell_statuses(cell, x, y);
                draw_cell_items(cell, x, y);
            }
            else
            {
                DrawRectangle(offset_x + x * draw_scale, y * draw_scale,
                              draw_scale, draw_scale, BLACK);
            }

            DrawRectangleLines(offset_x + x * draw_scale, y * draw_scale,
                               draw_scale, draw_scale, GRAY);
        }
    }

    if (player.if_alive())
    {
        DrawTexturePro(
            player_texture,
            {0, 0, (float)player_texture.width, (float)player_texture.height},
            {(float)(offset_x + p_x * draw_scale), (float)(p_y * draw_scale),
             (float)draw_scale, (float)draw_scale},
            {0, 0}, 0, WHITE);
    }

    for (Robot *enemy : enemies)
    {
        if (enemy == nullptr || !enemy->if_alive())
        {
            continue;
        }
        int e_x = enemy->get_x();
        int e_y = enemy->get_y();
        if (std::abs(e_x - p_x) + std::abs(e_y - p_y) < rad ||
            map.get_cell(e_x, e_y).if_visible())
        {
            DrawTexturePro(
                enemy_texture,
                {0, 0, (float)enemy_texture.width, (float)enemy_texture.height},
                {(float)(offset_x + e_x * draw_scale),
                 (float)(e_y * draw_scale), (float)draw_scale,
                 (float)draw_scale},
                {0, 0}, 0, WHITE);
            if (view_modes.enemies)
            {
                DrawRectangle(offset_x + e_x * draw_scale, e_y * draw_scale,
                              draw_scale, draw_scale, Fade(RED, 0.4f));
            }
        }
    }

    draw_ap_text(world);
    draw_abilities(world, abilities, view_modes);
    draw_player_bars(world);
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

void Render::init()
{
    InitWindow(1200, 1200, "Game");
    SetExitKey(KEY_NULL);

    around_texture = LoadTexture("assets/ab/around.png");
    range_texture = LoadTexture("assets/ab/range.png");
    heal_texture = LoadTexture("assets/ab/heal.png");
    teleport_texture = LoadTexture("assets/ab/teleport.png");

    player_texture = LoadTexture("assets/robots/player.png");
    enemy_texture = LoadTexture("assets/robots/enemy.png");

    grass_texture = LoadTexture("assets/bg/grass.png");

    SetTargetFPS(60);
    ToggleFullscreen();
    center_window();
}

void Render::shutdown()
{
    UnloadTexture(around_texture);
    UnloadTexture(range_texture);
    UnloadTexture(heal_texture);
    UnloadTexture(teleport_texture);
    UnloadTexture(player_texture);
    UnloadTexture(enemy_texture);
    UnloadTexture(grass_texture);
    CloseWindow();
}

void Render::center_window()
{
    int monitor = GetCurrentMonitor();
    int x = (GetMonitorWidth(monitor) - GetScreenWidth()) / 2;
    int y = (GetMonitorHeight(monitor) - GetScreenHeight()) / 2;
    SetWindowPosition(x, y);
}

void Render::toggle_fullscreen()
{
    if (IsKeyPressed(KEY_F11))
    {
        ToggleFullscreen();
        center_window();
    }
}

void Render::draw_abilities(World &world, std::vector<Ability *> abilities,
                            ViewModeState &view_modes)
{
    Map &map = world.get_map();
    Robot &player = world.get_player_ref();
    int panel_y = map.get_height() * draw_scale;
    int panel_height = UI_PANEL_HEIGHT;

    int square_size = UI_ABILITY_SQUARE;
    int spacing = UI_ABILITY_SPACING;

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

    int item_total = square_size * ITEM_COUNT + spacing * (ITEM_COUNT - 1);
    int item_start_x = start_x - item_total - 50;

    const char *item_names[] = {"Health", "Mana", "Attack"};
    Color item_colors[] = {RED, BLUE, ORANGE};

    for (int i = 0; i < ITEM_COUNT; i++)
    {
        int x = item_start_x + i * (square_size + spacing);
        int center_x = x + square_size / 2;

        DrawRectangleLines(x, panel_y + 10, square_size, square_size, WHITE);
        DrawCircle(center_x, panel_y + 34, 18, item_colors[i]);

        std::string count =
            "x" + std::to_string(player.get_item_count((ItemType)i));
        int count_width = MeasureText(count.c_str(), 16);
        DrawText(count.c_str(), center_x - count_width / 2, panel_y + 56, 16,
                 WHITE);

        int name_width = MeasureText(item_names[i], 16);
        DrawText(item_names[i], center_x - name_width / 2, panel_y + 90, 16,
                 WHITE);
    }

    draw_view_modes(panel_y, view_modes);
}

void Render::draw_view_modes(int panel_y, ViewModeState &view_modes)
{
    const char *names[] = {"Standard", "Enemies", "Impassable", "Statuses"};
    Color colors[] = {DARKGRAY, Fade(RED, 0.6f), Fade(ORANGE, 0.6f),
                      Fade(SKYBLUE, 0.6f)};

    bool active[] = {!view_modes.enemies && !view_modes.impassable &&
                         !view_modes.statuses,
                     view_modes.enemies, view_modes.impassable,
                     view_modes.statuses};

    for (int i = 0; i < VIEW_MODE_COUNT; i++)
    {
        Rectangle button = view_mode_button(i, GetScreenWidth(), panel_y);

        DrawRectangleRec(button, active[i] ? colors[i] : BLACK);
        DrawRectangleLinesEx(button, active[i] ? 2.0f : 1.0f,
                             active[i] ? WHITE : GRAY);

        int text_width = MeasureText(names[i], 16);
        DrawText(names[i], (int)(button.x + (button.width - text_width) / 2),
                 (int)(button.y + (button.height - 16) / 2), 16, WHITE);
    }
}

void Render::draw_player_bars(World &world)
{
    Map &map = world.get_map();
    Robot &player = world.get_player_ref();

    int panel_y = map.get_height() * draw_scale + 112;
    int bar_width = 340;
    int bar_height = 18;
    int spacing = 4;

    int x = (GetScreenWidth() - bar_width) / 2;
    int y = panel_y;

    draw_bar(x, y, bar_width, bar_height, player.get_hp(), player.get_max_hp(),
             RED, "HP");
    y += bar_height + spacing;

    draw_bar(x, y, bar_width, bar_height, player.get_mana(),
             player.get_max_mana(), BLUE, "MANA");
    y += bar_height + spacing;

    draw_bar(x, y, bar_width, bar_height, player.get_ap(), player.get_max_ap(),
             YELLOW, "AP");
    y += bar_height + spacing;

    std::string shield = "";
    std::vector<std::string> labels = player.get_status_labels();
    if (!labels.empty())
    {
        for (size_t i = 0; i < labels.size(); i++)
        {
            if (i > 0)
            {
                shield += ", ";
            }
            shield += labels[i];
        }
    }

    int shield_width = MeasureText(shield.c_str(), 16);
    DrawText(shield.c_str(), x + (bar_width - shield_width) / 2, y, 16, WHITE);
}

void Render::draw_bar(int x, int y, int width, int height, int value,
                      int max_value, Color color, std::string label)
{
    float percent = 0;
    if (max_value > 0)
    {
        percent = (float)value / max_value;
    }
    if (percent > 1)
    {
        percent = 1;
    }
    if (percent < 0)
    {
        percent = 0;
    }

    DrawRectangle(x, y, width, height, DARKGRAY);
    DrawRectangle(x, y, (int)(width * percent), height, color);
    DrawRectangleLines(x, y, width, height, WHITE);

    std::string text =
        label + ": " + std::to_string(value) + "/" + std::to_string(max_value);
    int text_width = MeasureText(text.c_str(), 16);
    DrawText(text.c_str(), x + (width - text_width) / 2, y + (height - 16) / 2,
             16, WHITE);
}

int Render::draw_statuses(int panel_x, int y, Robot &robot)
{
    std::vector<std::string> labels = robot.get_status_labels();
    for (size_t i = 0; i < labels.size(); i++)
    {
        DrawText(labels[i].c_str(), panel_x + 20, y + (int)i * 22, 16, ORANGE);
    }
    return (int)labels.size() * 22;
}

void Render::draw_cell_statuses(Cell &cell, int x, int y)
{
    std::string letters = cell.get_status_letters();
    if (letters.empty())
    {
        return;
    }
    int size = draw_scale / 3;
    if (size < 10)
    {
        size = 10;
    }
    DrawText(letters.c_str(), offset_x + x * draw_scale + 2, y * draw_scale + 2,
             size, RED);
}

void Render::draw_cell_items(Cell &cell, int x, int y)
{
    if (!cell.has_items())
    {
        return;
    }
    Color colors[] = {RED, BLUE, ORANGE};
    int radius = draw_scale / 8;
    if (radius < 3)
    {
        radius = 3;
    }

    int cell_x = offset_x + x * draw_scale;
    int margin = radius + 2;
    int center_y = y * draw_scale + draw_scale - radius - 3;

    int positions[ITEM_COUNT];
    positions[ITEM_HEALTH] = cell_x + draw_scale - margin;
    positions[ITEM_MANA] = cell_x + margin;
    positions[ITEM_ATTACK] = cell_x + draw_scale / 2;

    for (int i = 0; i < ITEM_COUNT; i++)
    {
        if (cell.get_item_count((ItemType)i) > 0)
        {
            DrawCircle(positions[i], center_y, radius, colors[i]);
        }
    }
}

void Render::draw_cell_highlights(Cell &cell, int x, int y,
                                  ViewModeState &view_modes)
{
    int pixel_x = offset_x + x * draw_scale;
    int pixel_y = y * draw_scale;

    if (view_modes.impassable && !cell.can_walk())
    {
        DrawRectangle(pixel_x, pixel_y, draw_scale, draw_scale,
                      Fade(ORANGE, 0.45f));
    }
    if (view_modes.statuses && cell.get_status_count() > 0)
    {
        DrawRectangle(pixel_x, pixel_y, draw_scale, draw_scale,
                      Fade(SKYBLUE, 0.45f));
    }
}

void Render::draw_ap_text(World &world)
{
    Map &map = world.get_map();
    std::vector<Robot *> &enemies = world.get_enemies_list();

    int panel_width = 200;
    int panel_x = GetScreenWidth() - panel_width;

    DrawRectangle(panel_x, 0, panel_width, map.get_height() * draw_scale,
                  BLACK);

    int y = 20;
    for (size_t i = 0; i < enemies.size(); i++)
    {
        if (enemies[i]->get_hp() > 0)
        {
            std::string name = "ENEMY " + std::to_string(i + 1);

            int bar_width = 170;
            int bar_height = 18;
            int spacing = 4;
            int bar_x = panel_x + panel_width - 10 - bar_width;

            DrawText(name.c_str(), bar_x, y, 20, WHITE);
            y += 26;

            draw_bar(bar_x, y, bar_width, bar_height, enemies[i]->get_hp(),
                     enemies[i]->get_max_hp(), RED, "HP");
            y += bar_height + spacing;

            draw_bar(bar_x, y, bar_width, bar_height, enemies[i]->get_mana(),
                     enemies[i]->get_max_mana(), BLUE, "MANA");
            y += bar_height + spacing;

            draw_bar(bar_x, y, bar_width, bar_height, enemies[i]->get_ap(),
                     enemies[i]->get_max_ap(), YELLOW, "AP");
            y += bar_height + spacing;

            y += 24 + draw_statuses(panel_x, y, *enemies[i]);
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
        DrawText("Press ENTER", width / 2 - 80, height / 2 + 50, 20, WHITE);
    }
    else if (game_over == 3)
    {
        DrawText("LEVEL COMPLETE", width / 2 - 300, height / 2 - 60, 60,
                 LIME);
        DrawText("Press ENTER", width / 2 - 80, height / 2 + 50, 20, WHITE);
    }
    else
    {
        DrawText("DEFEAT", width / 2 - 200, height / 2 - 60, 80, RED);
        DrawText("Press ENTER", width / 2 - 80, height / 2 + 50, 20, WHITE);
    }
}

int Render::get_draw_scale() { return draw_scale; }

int Render::get_offset_x() { return offset_x; }

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

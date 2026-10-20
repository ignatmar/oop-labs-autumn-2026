#pragma once

#include "ability.hpp"
#include "map.hpp"
#include "world.hpp"
#include <raylib.h>

class Render
{
  private:
    int draw_scale;
    int offset_x;

    int draw_statuses(int panel_x, int y, Robot &robot);
    void draw_cell_statuses(Cell &cell, int x, int y);
    void draw_player_bars(World &world);
    void draw_bar(int x, int y, int width, int height, int value, int max_value,
                  Color color, std::string label);

    Texture2D around_texture;
    Texture2D range_texture;
    Texture2D heal_texture;
    Texture2D teleport_texture;

    Texture2D player_texture;
    Texture2D enemy_texture;

    Texture2D grass_texture;

  public:
    Render(int draw_scale);

    void draw(World &world, int game_over, int level_up,
              std::vector<Ability *> abilities);
    void start(Map &map);
    void draw_ap_text(World &world);
    void draw_game_over(int game_over);
    void toggle_fullscreen();
    void center_window();

    void draw_abilities(Map &map, std::vector<Ability *> abilities);

    void draw_level_up();

    int get_draw_scale();
    int get_offset_x();
};

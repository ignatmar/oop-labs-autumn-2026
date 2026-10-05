#pragma once

#include "ability.hpp"
#include "map.hpp"
#include <raylib.h>

class Render
{
  private:
    int draw_scale;

    Texture2D around_texture;
    Texture2D range_texture;
    Texture2D heal_texture;
    Texture2D teleport_texture;

    Texture2D player_texture;
    Texture2D enemy_texture;

    Texture2D grass_texture;

  public:
    Render(int draw_scale);

    void draw(Map &map, Robot &player, std::vector<Robot *> enemy_robots,
              int game_over, int level_up, std::vector<Ability *> abilities);
    void start(Map &map);
    void draw_ap_text(Map &map, Robot &player,
                      const std::vector<Robot *> enemy_robots);
    void draw_game_over(int game_over);

    void draw_abilities(Map &map, std::vector<Ability *> abilities);

    void draw_level_up();

    int get_level_up_choice();

    int get_draw_scale();
};

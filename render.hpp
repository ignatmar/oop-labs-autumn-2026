#pragma once

#include "map.hpp"
#include <raylib.h>

class Render
{
  private:
    int draw_scale;

  public:
    Render(int draw_scale);

    void draw(Map &map, Robot &player, std::vector<Robot *> enemy_robots,
              int game_over);
    void start(Map &map);
    void draw_ap_text(Map &map, Robot &player,
                      const std::vector<Robot *> enemy_robots);
    void draw_game_over(int game_over);
};

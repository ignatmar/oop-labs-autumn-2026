#pragma once
#include "levels/level_data.hpp"
#include "robot/robot.hpp"
#include "world/world.hpp"

class Level
{
  private:
    Robot *player;
    World *world;

    Status *create_status(StatusSpawn &spawn);

  public:
    Level(LevelData &data, PlayerProfile &profile);
    ~Level();
    World &get_world();
    Robot &get_player();
    PlayerProfile make_profile();
};

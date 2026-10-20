#pragma once
#include "levels/level_data.hpp"
#include "robot/robot.hpp"
#include "world/world.hpp"
#include <memory>

class Level
{
  private:
    std::unique_ptr<Robot> player;
    std::unique_ptr<World> world;

    Status *create_status(StatusSpawn &spawn);

  public:
    Level(LevelData &data, PlayerProfile &profile);
    World &get_world();
    Robot &get_player();
    PlayerProfile make_profile();
};

#pragma once
#include "levels/level_data.hpp"
#include <string>
#include <vector>

class FixedLevels
{
  private:
    std::vector<LevelData> levels;

    void load_file(std::string path);

  public:
    FixedLevels(std::string directory);
    int count();
    bool has_next(int index);
    LevelData make_level(int index);
    GameMode mode();
    unsigned get_seed();
};

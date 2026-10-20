#pragma once
#include "levels/level_data.hpp"

class RandomLevelGenerator
{
  private:
    unsigned seed;

  public:
    RandomLevelGenerator(unsigned seed);
    LevelData make(int index);
};

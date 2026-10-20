#pragma once
#include "levels/level_data.hpp"
#include "levels/random_levels/random_level_generator.hpp"

template <int N>
class RandomLevels
{
  private:
    RandomLevelGenerator generator;
    unsigned seed;

  public:
    RandomLevels(unsigned seed) : generator(seed), seed(seed) {}

    int count() { return N; }

    bool has_next(int index) { return index + 1 < N; }

    LevelData make_level(int index) { return generator.make(index); }

    GameMode mode() { return MODE_RANDOM; }

    unsigned get_seed() { return seed; }
};

class EndlessLevels
{
  private:
    RandomLevelGenerator generator;
    unsigned seed;

  public:
    EndlessLevels(unsigned seed) : generator(seed), seed(seed) {}

    int count() { return -1; }

    bool has_next(int) { return true; }

    LevelData make_level(int index) { return generator.make(index); }

    GameMode mode() { return MODE_ENDLESS; }

    unsigned get_seed() { return seed; }
};

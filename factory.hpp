#pragma once

#include "map.hpp"

class Factory
{
  private:
    int x, y;
    int freq;

  public:
    Factory(int x, int y, int freq);
    void spawn(Map &map);
};

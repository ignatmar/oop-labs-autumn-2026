#pragma once

class World;

class Factory
{
  private:
    int x, y;
    int freq;

  public:
    Factory(int x, int y, int freq);
    void spawn(World &world);
};

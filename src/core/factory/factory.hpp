#pragma once

class World;

class Factory
{
  private:
    int x, y;
    int freq;
    int rank;

  public:
    Factory(int x, int y, int freq, int rank);
    void spawn(World &world);
};

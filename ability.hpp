#pragma once
#include <cstdlib>
class Robot;
class Map;

class Ability
{
  public:
    virtual void use(Robot &robot, Map &map, size_t x, size_t y) = 0;
    virtual bool can_use(Robot &robot, Map &map, size_t x, size_t y) = 0;
    virtual int get_cost() = 0;
    virtual void upgrade() = 0;
};

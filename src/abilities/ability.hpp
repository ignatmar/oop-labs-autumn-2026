#pragma once
#include <cstddef>
class Robot;
class World;

class Ability
{
  public:
    virtual void use(Robot &robot, World &world, size_t x, size_t y) = 0;
    virtual bool can_use(Robot &robot, World &world, size_t x, size_t y) = 0;
    virtual int get_cost() = 0;
    virtual void upgrade() = 0;
    virtual bool is_unlocked() = 0;
    virtual void unlock() = 0;
    virtual ~Ability()
    {
    }
};

#pragma once
#include "ability.hpp"

class Range_ab : public Ability
{
  private:
    int range;
    int damage;
    int cost;
    int level;
    bool unlocked;

  public:
    Range_ab(int radius, int damage, int cost);

    void use(Robot &robot, World &world, size_t x, size_t y) override;
    bool can_use(Robot &robot, World &world, size_t x, size_t y) override;
    int get_cost() override;
    void upgrade() override;
    int get_level() override;

    bool is_unlocked() override;
    void unlock() override;
};

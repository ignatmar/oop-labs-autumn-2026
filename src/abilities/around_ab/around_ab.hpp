#pragma once
#include "ability.hpp"

class Around_ab : public Ability
{
  private:
    int radius;
    int damage;
    int cost;
    int level;
    bool unlocked;

  public:
    Around_ab(int radius, int damage, int cost);

    void use(Robot &robot, World &world, size_t, size_t) override;
    bool can_use(Robot &robot, World &, size_t, size_t) override;
    int get_cost() override;
    void upgrade() override;
    int get_level() override;

    bool is_unlocked() override;
    void unlock() override;
};

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

    void use(Robot &robot, Map &map, size_t, size_t) override;
    bool can_use(Robot &robot, Map &, size_t, size_t) override;
    int get_cost() override;
    void upgrade() override;

    bool is_unlocked() override;
    void unlock() override;
};

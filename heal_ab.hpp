#pragma once
#include "ability.hpp"

class Heal_ab : public Ability
{
  private:
    int range;
    int delta_hp;
    int cost;
    int level;
    bool unlocked;

  public:
    Heal_ab(int radius, int delta_hp, int cost);

    void use(Robot &robot, Map &map, size_t x, size_t y) override;
    bool can_use(Robot &robot, Map &map, size_t x, size_t y) override;
    int get_cost() override;
    void upgrade() override;

    bool is_unlocked() override;
    void unlock() override;
};

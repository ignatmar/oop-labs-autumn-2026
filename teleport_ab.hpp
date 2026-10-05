#pragma once
#include "ability.hpp"

class Teleport_ab : public Ability
{
  private:
    int range;
    int damage;
    int cost;
    int level;

  public:
    Teleport_ab(int radius, int damage, int cost);

    void use(Robot &robot, Map &map, size_t x, size_t y) override;
    bool can_use(Robot &robot, Map &map, size_t x, size_t y) override;
    int get_cost() override;
    void upgrade() override;
};

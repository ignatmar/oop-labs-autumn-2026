#pragma once
#include "ability.hpp"

class Teleport_ab : public Ability
{
  private:
    int range;
    int damage;
    int cost;
    int level;
    bool unlocked;

    bool is_free_cell(Robot &robot, World &world, size_t x, size_t y);
    int in_range(int x, int y, int from_x, int from_y, int range);

  public:
    Teleport_ab(int radius, int damage, int cost);

    void use(Robot &robot, World &world, size_t x, size_t y) override;
    bool can_use(Robot &robot, World &world, size_t x, size_t y) override;
    int get_cost() override;
    void upgrade() override;
    int get_level() override;

    bool is_unlocked() override;
    void unlock() override;

    int get_range();
    int find_target(Robot &robot, World &world, size_t &x, size_t &y);
};

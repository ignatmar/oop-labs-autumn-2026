#pragma once
#include "status.hpp"

class Dmg_boost : public Status
{
  private:
    int turns;
    int damage;

  public:
    Dmg_boost(int turns, int damage);

    void apply(Robot &robot) override;
    void update(Robot &robot) override;
    bool is_active() override;
    StatusType get_type() override;
    void combine(Status *status) override;
};

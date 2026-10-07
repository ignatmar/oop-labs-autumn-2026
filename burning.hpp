#pragma once
#include "status.hpp"

class Burning : public Status
{
  private:
    int turns;
    int damage;

  public:
    Burning(int turns, int damage);
    void apply(Robot &robot) override;
    void update(Robot &robot) override;
    bool is_active() override;
    void combine(Status *status) override;
    StatusType get_type() override;
};

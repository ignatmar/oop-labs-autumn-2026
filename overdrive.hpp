#pragma once
#include "status.hpp"

class Overdrive : public Status
{
  private:
    int turns;

  public:
    Overdrive();
    void apply(Robot &robot) override;
    void update(Robot &robot) override;
    bool is_active() override;
    void combine(Status *status) override;
    StatusType get_type() override;
};

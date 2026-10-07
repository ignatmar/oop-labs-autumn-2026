#pragma once
#include "status.hpp"

class Slow : public Status
{
  private:
    int turns;

  public:
    Slow();
    void apply(Robot &robot) override;
    void update(Robot &robot) override;
    bool is_active() override;
    void combine(Status *status) override;
    StatusType get_type() override;
};

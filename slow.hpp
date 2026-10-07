#pragma once
#include "status.hpp"

class Slow : public Status
{
  private:
    int turns;

  public:
    Slow();
    void apply(Robot &) override;
    void update(Robot &) override;
    bool is_active() override;
    void combine(Status *status) override;
    StatusType get_type() override;
};

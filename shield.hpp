#pragma once
#include "status.hpp"

class Shield : public Status
{
  private:
    int turns;
    int damage;

  public:
    Shield(int damage);
    void apply(Robot &robot) override;
    void update(Robot &robot) override;
    bool is_active() override;
    void combine(Status *status) override;
    StatusType get_type() override;
    int get_damage();
    void set_damage(int damage);
};

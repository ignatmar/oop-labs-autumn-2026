#pragma once
#include "status.hpp"
#include <string>

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
    std::string get_label() override;
    char get_symbol() override;
    Status *clone() override;
    int get_damage();
    void set_damage(int damage);
};

#pragma once
#include "status.hpp"
#include <string>

class Slow : public Status
{
  private:
    int turns;
    bool fresh;

  public:
    Slow();
    void apply(Robot &) override;
    void update(Robot &) override;
    bool is_active() override;
    void combine(Status *status) override;
    StatusType get_type() override;
    std::string get_label() override;
    char get_symbol() override;
    Status *clone() override;

    bool is_fresh();
    void make_ready();
};

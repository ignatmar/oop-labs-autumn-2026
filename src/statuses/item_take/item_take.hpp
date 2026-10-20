#pragma once
#include "item/item.hpp"
#include "status.hpp"
#include <string>

class ItemTake : public Status
{
  private:
    ItemType type;
    int count;
    int turns;

  public:
    ItemTake(ItemType type, int count);
    void apply(Robot &robot) override;
    void update(Robot &robot) override;
    bool is_active() override;
    void combine(Status *status) override;
    StatusType get_type() override;
    std::string get_label() override;
    char get_symbol() override;
    Status *clone() override;
};

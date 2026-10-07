#pragma once

class Robot;

enum StatusType
{
    SLOW,
    OVERDRIVE,
    BURNING,
    SHIELD,
    DMGBOOST,
    ITEM
};

class Status
{
  public:
    virtual ~Status() = default;
    virtual void apply(Robot &robot) = 0;
    virtual void update(Robot &robot) = 0;
    virtual bool is_active() = 0;
    virtual StatusType get_type() = 0;
    virtual void combine(Status *status) = 0;
};

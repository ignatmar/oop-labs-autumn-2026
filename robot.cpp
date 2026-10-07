#include "robot.hpp"
#include "dmg_boost.hpp"
#include "shield.hpp"
int Robot::normalize(int x, int min_x, int max_x)
{
    if (x < min_x)
    {
        return min_x;
    }
    if (x > max_x)
    {
        return max_x;
    }
    return x;
}

Robot::Robot(int hp_max, int dmg, int mana_max, int speed, int exp_max,
             int team, int vision, bool visible)
    : hp(hp_max), hp_max(hp_max), dmg(dmg), mana(mana_max), mana_max(mana_max),
      exp(0), exp_max(exp_max), lvl(0), x(0), y(0), team(team), ap(speed * 2),
      ap_max(speed * 2), kills(0), speed(speed), vision(vision),
      visible(visible)
{
}

void Robot::set_hp(int new_hp) { hp = normalize(new_hp, 0, hp_max); }

void Robot::set_max_hp(int new_max_hp)
{
    hp_max = normalize(new_max_hp, 0, new_max_hp);
    if (hp > hp_max)
    {
        hp = hp_max;
    }
}

void Robot::set_dmg(int new_dmg) { dmg = normalize(new_dmg, 0, new_dmg); }

void Robot::set_mana(int new_mana) { mana = normalize(new_mana, 0, mana_max); }

void Robot::set_max_mana(int new_max_mana)
{
    mana_max = normalize(new_max_mana, 0, new_max_mana);
    if (mana > mana_max)
    {
        mana = mana_max;
    }
}

int Robot::add_exp(int delta_exp)
{
    exp += delta_exp;

    int levels = 0;

    while (exp >= exp_max)
    {
        exp -= exp_max;
        lvl_up();
        levels++;
    }
    if (team == 1)
    {
        return levels;
    }
    return 0;
}

int Robot::get_lvl() { return lvl; }

void Robot::lvl_up()
{
    lvl++;
    hp_max += 100;
    dmg += 30;
    mana_max += 50;
    exp_max += 80;
}

void Robot::upd_hp(int delta_hp)
{
    if (delta_hp >= 0)
    {
        set_hp(hp + delta_hp);
        return;
    }

    int damage = -delta_hp;

    for (auto it = statuses.begin(); it != statuses.end(); ++it)
    {
        if ((*it)->get_type() == SHIELD)
        {
            Shield *shield = (Shield *)*it;

            if (shield->get_damage() >= damage)
            {
                shield->set_damage(shield->get_damage() - damage);

                if (!shield->is_active())
                {
                    delete *it;
                    statuses.erase(it);
                }

                return;
            }

            damage -= shield->get_damage();
            delete *it;
            statuses.erase(it);
            break;
        }
    }

    set_hp(hp - damage);
}

void Robot::interact(Robot &other)
{
    if (team == other.team)
    {
        other.upd_hp(dmg);
    }
    else
    {
        other.upd_hp(-dmg);
    }
}

void Robot::set_coords(int x, int y)
{
    this->x = x;
    this->y = y;
}

int Robot::get_x() { return x; }

int Robot::get_y() { return y; }

int Robot::get_team() { return team; }

void Robot::set_ap(int new_ap) { ap = normalize(new_ap, 0, ap_max); }

void Robot::set_max_ap(int new_max_ap)
{
    ap_max = normalize(new_max_ap, 0, new_max_ap);
    if (ap > ap_max)
    {
        ap = ap_max;
    }
}

int Robot::get_ap() { return ap; }

int Robot::get_hp() { return hp; }

int Robot::get_max_hp() { return hp_max; }

int Robot::get_max_ap() { return ap_max; }

size_t Robot::get_kills() { return kills; }

void Robot::add_kill() { kills++; }

int Robot::get_vision() { return vision; }

int Robot::get_mana() { return mana; }

int Robot::get_max_mana() { return mana_max; }

int Robot::get_max_exp() { return exp_max; };

void Robot::add_status(Status *status)
{
    for (Status *current : statuses)
    {
        if (current->get_type() == status->get_type())
        {
            current->combine(status);
            delete status;
            return;
        }
    }

    statuses.push_back(status);
}

void Robot::update_statuses()
{
    for (auto it = statuses.begin(); it != statuses.end();)
    {
        (*it)->update(*this);

        if (!(*it)->is_active())
        {
            delete *it;
            it = statuses.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool Robot::can_move()
{
    for (Status *status : statuses)
    {
        if (status->get_type() == SLOW)
        {
            return false;
        }
    }

    return true;
}

bool Robot::can_use_ability()
{
    for (auto it = statuses.begin(); it != statuses.end(); ++it)
    {
        if ((*it)->get_type() == OVERDRIVE)
        {
            delete *it;
            statuses.erase(it);
            return false;
        }
    }

    return true;
}

int Robot::get_shield()
{
    for (Status *status : statuses)
    {
        if (status->get_type() == SHIELD)
        {
            Shield *shield = (Shield *)status;
            return shield->get_damage();
        }
    }

    return 0;
}

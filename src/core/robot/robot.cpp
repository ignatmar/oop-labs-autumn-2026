#include "robot/robot.hpp"
#include "attack_boost/attack_boost.hpp"
#include "map/map.hpp"
#include "shield/shield.hpp"
#include "slow/slow.hpp"
#include "world/world.hpp"
#include <raylib.h>
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
      visible(visible), alive(true), ability(ABILITY_NONE), items{}
{
}

Robot::~Robot()
{
    for (Status *status : statuses)
    {
        delete status;
    }
}

void Robot::set_hp(int new_hp)
{
    hp = normalize(new_hp, 0, hp_max);
    if (hp <= 0)
    {
        alive = false;
    }
}

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

void Robot::set_lvl(int new_lvl) { lvl = new_lvl; }

void Robot::lvl_up()
{
    lvl++;
    hp_max += 100;
    dmg += 2;
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

int Robot::interact(Robot &other, int damage)
{
    if (team == other.team)
    {
        other.upd_hp(damage);
    }
    else
    {
        other.upd_hp(-damage);
        if (other.get_hp() <= 0)
        {
            other.make_dead();
            add_kill();
            int levels = add_exp(get_max_exp() / 2);
            TraceLog(LOG_INFO, "Robot::interact returns LEVELS: %d", levels);
            if (levels > 0)
            {
                return levels;
            }
            else
            {
                return 0;
            }
        }
    }
    return -1;
}

void Robot::teleport_to(World &world, int new_x, int new_y)
{
    if (new_x == x && new_y == y)
    {
        return;
    }
    world.get_map().get_cell(x, y).set_busy(false);
    x = new_x;
    y = new_y;
    world.get_map().get_cell(x, y).set_busy(true);
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

int Robot::get_exp() { return exp; }

void Robot::set_exp(int new_exp) { exp = new_exp; }

int Robot::get_speed() { return speed; }

int Robot::get_max_exp() { return exp_max; };

void Robot::clear_statuses()
{
    for (Status *status : statuses)
    {
        delete status;
    }
    statuses.clear();
}

void Robot::add_status(Status *status)
{
    if (status == nullptr)
    {
        return;
    }
    status->apply(*this);

    if (status->get_type() != ITEM_TAKE && status->get_type() != ITEM_USE)
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
    }

    statuses.push_back(status);
}

void Robot::update_statuses()
{
    for (auto it = statuses.begin(); it != statuses.end();)
    {
        if ((*it)->get_type() != SLOW && (*it)->get_type() != OVERDRIVE)
        {
            (*it)->update(*this);
        }

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

std::vector<std::string> Robot::get_status_labels()
{
    std::vector<std::string> labels;
    for (Status *status : statuses)
    {
        if (status->is_active())
        {
            labels.push_back(status->get_label());
        }
    }
    return labels;
}

bool Robot::can_move()
{
    return true;
}

void Robot::consume_slow()
{
    for (auto it = statuses.begin(); it != statuses.end(); ++it)
    {
        if ((*it)->get_type() != SLOW)
        {
            continue;
        }
        Slow *slow = (Slow *)*it;
        slow->update(*this);
        if (!slow->is_active())
        {
            delete slow;
            statuses.erase(it);
        }
        return;
    }
}

bool Robot::is_slowed()
{
    for (Status *status : statuses)
    {
        if (status->get_type() == SLOW && status->is_active())
        {
            Slow *slow = (Slow *)status;
            return !slow->is_fresh();
        }
    }
    return false;
}

void Robot::ready_slow()
{
    for (Status *status : statuses)
    {
        if (status->get_type() == SLOW)
        {
            ((Slow *)status)->make_ready();
        }
    }
}

bool Robot::try_use_ability()
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

void Robot::set_ability(int new_ability) { ability = new_ability; }

int Robot::get_ability() { return ability; }

void Robot::clear_ability() { ability = ABILITY_NONE; }

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

int Robot::get_dmg()
{
    int total = dmg;
    for (Status *status : statuses)
    {
        if (status->get_type() == ATTACK_BOOST)
        {
            total += ((AttackBoost *)status)->get_bonus();
        }
    }
    return total;
}

int Robot::get_base_dmg() { return dmg; }

void Robot::add_item(ItemType type, int count)
{
    if (type >= 0 && type < ITEM_COUNT)
    {
        items[type] += count;
    }
}

void Robot::use_item(ItemType type)
{
    if (type < 0 || type >= ITEM_COUNT || items[type] <= 0)
    {
        return;
    }
    items[type]--;
    if (type == ITEM_HEALTH)
    {
        upd_hp(50);
    }
    else if (type == ITEM_MANA)
    {
        set_mana(mana + 50);
    }
    else if (type == ITEM_ATTACK)
    {
        add_status(new AttackBoost(10));
    }
}

int Robot::get_item_count(ItemType type)
{
    if (type < 0 || type >= ITEM_COUNT)
    {
        return 0;
    }
    return items[type];
}

int Robot::get_items()
{
    int total = 0;
    for (int i = 0; i < ITEM_COUNT; i++)
    {
        total += items[i];
    }
    return total;
}

void Robot::make_dead() { alive = false; }

bool Robot::if_alive() { return alive; }

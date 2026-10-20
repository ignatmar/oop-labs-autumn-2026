#pragma once
#include "item/item.hpp"
#include "status.hpp"
#include <cstddef>
#include <string>
#include <vector>

class World;

enum EnemyAbility
{
    ABILITY_NONE = 0,
    ABILITY_HEAL = 1,
    ABILITY_TELEPORT = 2
};

class Robot
{
  private:
    int hp;
    int hp_max;
    int dmg;
    int mana;
    int mana_max;
    int exp;
    int exp_max;
    int lvl;
    int x, y;
    int team;
    int ap;
    int ap_max;
    size_t kills;
    int speed;
    int vision;
    bool visible;

    bool alive;

    int ability;

    int normalize(int x, int min_x, int max_x);

    std::vector<Status *> statuses;

    int items[ITEM_COUNT];

  public:
    Robot(int hp_max, int dmg, int mana_max, int speed, int exp_max, int team,
          int vision, bool visible);
    ~Robot();
    Robot(const Robot &other) = delete;
    Robot &operator=(const Robot &other) = delete;
    void set_hp(int new_hp);
    void set_max_hp(int new_max_hp);
    void set_dmg(int new_dmg);
    void set_mana(int new_mana);
    void set_max_mana(int new_max_mana);
    int add_exp(int delta_exp);
    int get_max_exp();
    int get_exp();
    void set_exp(int new_exp);
    int get_lvl();
    void set_lvl(int new_lvl);
    void lvl_up();
    void upd_hp(int delta_hp);
    int interact(Robot &other, int damage);
    void set_coords(int x, int y);
    void teleport_to(World &world, int new_x, int new_y);
    int get_x();
    int get_y();
    int get_team();
    void set_ap(int new_ap);
    void set_max_ap(int new_max_ap);
    int get_ap();
    int get_hp();
    int get_max_ap();
    int get_max_hp();
    size_t get_kills();
    void add_kill();
    int get_vision();
    int get_mana();
    int get_max_mana();

    void add_status(Status *status);
    void clear_statuses();
    void update_statuses();
    std::vector<std::string> get_status_labels();
    void set_ability(int new_ability);
    int get_ability();
    void clear_ability();
    bool try_use_ability();
    bool can_move();
    bool is_slowed();
    void consume_slow();
    void ready_slow();
    int get_shield();

    int get_dmg();
    int get_base_dmg();
    int get_speed();

    void add_item(ItemType type, int count);
    void use_item(ItemType type);
    int get_item_count(ItemType type);
    int get_items();

    void make_dead();
    bool if_alive();
};

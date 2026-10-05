#pragma once

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
    int kills;
    int speed;
    int vision;
    bool visible;
    int normalize(int x, int min_x, int max_x);

  public:
    Robot(int hp_max, int dmg, int mana_max, int speed, int exp_max, int team,
          int vision, bool visible);
    void set_hp(int new_hp);
    void set_max_hp(int new_max_hp);
    void set_dmg(int new_dmg);
    void set_mana(int new_mana);
    void set_max_mana(int new_max_mana);
    void set_exp(int new_exp);
    void set_lvl(int new_lvl);
    void lvl_up();
    void upd_hp(int delta_hp);
    void interact(Robot &other);
    void set_coords(int x, int y);
    int get_x();
    int get_y();
    int get_team();
    void set_ap(int new_ap);
    void set_max_ap(int new_max_ap);
    int get_ap();
    int get_hp();
    int get_max_ap();
    int get_max_hp();
    int get_kills();
    void add_kill();
    int get_vision();
    bool if_visible();
};

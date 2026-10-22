#pragma once

class Input
{
  public:
    struct MoveCmd
    {
        bool moved;
        int dx, dy;
    };

    struct AbilityCmd
    {
        bool used;
        int slot;
        int target_x;
        int target_y;
    };

    int get_level_up_choice();
    MoveCmd get_move();
    AbilityCmd get_ability(int draw_scale, int offset_x);
    int get_item();
    int get_view_mode();
    bool fullscreen_requested();
    bool menu_requested();
    bool confirm_requested();
};

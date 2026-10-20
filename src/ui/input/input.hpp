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

    static int get_level_up_choice();
    static MoveCmd get_move();
    static AbilityCmd get_ability(int draw_scale, int offset_x);
    static int get_item();
    static int get_view_mode();
    static bool fullscreen_requested();
    static bool menu_requested();
    static bool confirm_requested();
};

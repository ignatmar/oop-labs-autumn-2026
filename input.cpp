#include "input.hpp"
#include <raylib.h>

Input::MoveCmd Input::get_move()
{
    MoveCmd cmd{false, 0, 0};

    if (IsKeyPressed(KEY_W))
    {
        cmd.moved = true;
        cmd.dx = 0;
        cmd.dy = -1;
        return cmd;
    }
    if (IsKeyPressed(KEY_S))
    {
        cmd.moved = true;
        cmd.dx = 0;
        cmd.dy = 1;
        return cmd;
    }
    if (IsKeyPressed(KEY_A))
    {
        cmd.moved = true;
        cmd.dx = -1;
        cmd.dy = 0;
        return cmd;
    }
    if (IsKeyPressed(KEY_D))
    {
        cmd.moved = true;
        cmd.dx = 1;
        cmd.dy = 0;
        return cmd;
    }

    return cmd;
}

Input::AbilityCmd Input::get_ability(int draw_scale)
{
    AbilityCmd cmd{false, 0, 0, 0};

    if (IsKeyPressed(KEY_ONE))
    {
        cmd.used = true;
        cmd.slot = 1;
        cmd.target_x = 0;
        cmd.target_y = 0;
        return cmd;
    }

    if (IsKeyPressed(KEY_TWO))
    {
        Vector2 mouse = GetMousePosition();
        cmd.used = true;
        cmd.slot = 2;
        cmd.target_x = (int)(mouse.x / draw_scale);
        cmd.target_y = (int)(mouse.y / draw_scale);
        return cmd;
    }

    if (IsKeyPressed(KEY_THREE))
    {
        cmd.used = true;
        cmd.slot = 3;
        cmd.target_x = 0;
        cmd.target_y = 0;
        return cmd;
    }

    if (IsKeyPressed(KEY_FOUR))
    {
        Vector2 mouse = GetMousePosition();
        cmd.used = true;
        cmd.slot = 4;
        cmd.target_x = (int)(mouse.x / draw_scale);
        cmd.target_y = (int)(mouse.y / draw_scale);
        return cmd;
    }

    return cmd;
}

int Input::get_level_up_choice()
{
    int width = 700;
    int height = 700;

    int start_x = (GetScreenWidth() - width) / 2;
    int start_y = (GetScreenHeight() - height) / 2;

    int square_size = 250;
    int spacing = 30;

    int x1 = start_x + 70;
    int x2 = x1 + square_size + spacing;
    int y1 = start_y + 100;
    int y2 = y1 + square_size + spacing;

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return 0;

    Vector2 mouse = GetMousePosition();

    if (mouse.x >= x1 && mouse.x <= x1 + square_size && mouse.y >= y1 &&
        mouse.y <= y1 + square_size)
        return 1;
    if (mouse.x >= x2 && mouse.x <= x2 + square_size && mouse.y >= y1 &&
        mouse.y <= y1 + square_size)
        return 2;
    if (mouse.x >= x1 && mouse.x <= x1 + square_size && mouse.y >= y2 &&
        mouse.y <= y2 + square_size)
        return 3;
    if (mouse.x >= x2 && mouse.x <= x2 + square_size && mouse.y >= y2 &&
        mouse.y <= y2 + square_size)
        return 4;

    return 0;
}

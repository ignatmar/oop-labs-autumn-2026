#include "view_modes/view_modes.hpp"

Rectangle view_mode_button(int index, int screen_width, int panel_y)
{
    int total_width = UI_ABILITY_SQUARE * 4 + UI_ABILITY_SPACING * 3;
    int start_x = (screen_width - total_width) / 2;
    int mode_start_x = start_x + total_width + UI_MODE_GAP;

    int column = index % 2;
    int row = index / 2;

    return {(float)(mode_start_x +
                    column * (UI_MODE_BUTTON_WIDTH + UI_MODE_BUTTON_SPACING)),
            (float)(panel_y + 10 +
                    row * (UI_MODE_BUTTON_HEIGHT + UI_MODE_BUTTON_SPACING)),
            (float)UI_MODE_BUTTON_WIDTH, (float)UI_MODE_BUTTON_HEIGHT};
}

void toggle_view_mode(ViewModeState &state, int mode)
{
    if (mode == VIEW_STANDARD)
    {
        state.enemies = false;
        state.impassable = false;
        state.statuses = false;
    }
    else if (mode == VIEW_ENEMIES)
    {
        state.enemies = !state.enemies;
    }
    else if (mode == VIEW_IMPASSABLE)
    {
        state.impassable = !state.impassable;
    }
    else if (mode == VIEW_STATUSES)
    {
        state.statuses = !state.statuses;
    }
}

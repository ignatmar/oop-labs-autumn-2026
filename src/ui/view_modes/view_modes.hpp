#pragma once
#include <raylib.h>

enum ViewMode
{
    VIEW_STANDARD = 0,
    VIEW_ENEMIES = 1,
    VIEW_IMPASSABLE = 2,
    VIEW_STATUSES = 3,
    VIEW_MODE_COUNT = 4
};

struct ViewModeState
{
    bool enemies = false;
    bool impassable = false;
    bool statuses = false;
};

const int UI_PANEL_HEIGHT = 200;
const int UI_ABILITY_SQUARE = 70;
const int UI_ABILITY_SPACING = 35;
const int UI_MODE_GAP = 50;
const int UI_MODE_BUTTON_WIDTH = 120;
const int UI_MODE_BUTTON_HEIGHT = 38;
const int UI_MODE_BUTTON_SPACING = 10;

Rectangle view_mode_button(int index, int screen_width, int panel_y);
void toggle_view_mode(ViewModeState &state, int mode);

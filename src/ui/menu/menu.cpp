#include "menu/menu.hpp"
#include <raylib.h>

Menu::Menu() : message("") {}

void Menu::set_message(std::string text)
{
    message = text;
}

bool Menu::button(Rectangle rectangle, const char *text)
{
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rectangle);

    DrawRectangleRec(rectangle, hover ? DARKGRAY : BLACK);
    DrawRectangleLinesEx(rectangle, 2, hover ? WHITE : GRAY);

    int width = MeasureText(text, 24);
    DrawText(text, (int)(rectangle.x + (rectangle.width - width) / 2),
             (int)(rectangle.y + (rectangle.height - 24) / 2), 24, WHITE);

    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

MenuAction Menu::run()
{
    State state = MAIN;

    while (!WindowShouldClose())
    {
        int width = GetScreenWidth();
        int button_width = 320;
        int button_height = 52;
        int x = (width - button_width) / 2;

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("OOP LAB", width / 2 - MeasureText("OOP LAB", 60) / 2, 180,
                 60, WHITE);

        MenuAction action = MENU_NONE;

        if (state == MAIN)
        {
            if (button({(float)x, 420, (float)button_width,
                        (float)button_height},
                       "New Game"))
            {
                state = NEW_GAME;
            }
            if (button({(float)x, 490, (float)button_width,
                        (float)button_height},
                       "Load Game"))
            {
                action = MENU_LOAD;
            }
            if (button({(float)x, 560, (float)button_width,
                        (float)button_height},
                       "Exit"))
            {
                action = MENU_EXIT;
            }
        }
        else
        {
            DrawText("SELECT MODE", width / 2 - MeasureText("SELECT MODE", 30) / 2,
                     350, 30, WHITE);
            if (button({(float)x, 420, (float)button_width,
                        (float)button_height},
                       "Fixed Levels"))
            {
                action = MENU_NEW_FIXED;
            }
            if (button({(float)x, 490, (float)button_width,
                        (float)button_height},
                       "Random Levels"))
            {
                action = MENU_NEW_RANDOM;
            }
            if (button({(float)x, 560, (float)button_width,
                        (float)button_height},
                       "Endless Mode"))
            {
                action = MENU_NEW_ENDLESS;
            }
            if (button({(float)x, 630, (float)button_width,
                        (float)button_height},
                       "Back"))
            {
                state = MAIN;
            }
        }

        if (!message.empty())
        {
            int message_width = MeasureText(message.c_str(), 22);
            DrawText(message.c_str(), (width - message_width) / 2, 760, 22,
                     ORANGE);
        }

        EndDrawing();

        if (IsKeyPressed(KEY_ESCAPE))
        {
            if (state == NEW_GAME)
            {
                state = MAIN;
            }
            else
            {
                return MENU_EXIT;
            }
        }

        if (action != MENU_NONE)
        {
            return action;
        }
    }

    return MENU_EXIT;
}

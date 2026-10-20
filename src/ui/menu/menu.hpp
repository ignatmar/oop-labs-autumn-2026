#pragma once
#include <raylib.h>
#include <string>

enum MenuAction
{
    MENU_NONE = 0,
    MENU_NEW_FIXED = 1,
    MENU_NEW_RANDOM = 2,
    MENU_NEW_ENDLESS = 3,
    MENU_LOAD = 4,
    MENU_EXIT = 5
};

class Menu
{
  private:
    enum State
    {
        MAIN,
        NEW_GAME
    };

    std::string message;

    bool button(Rectangle rectangle, const char *text);

  public:
    Menu();
    MenuAction run();
    void set_message(std::string text);
};

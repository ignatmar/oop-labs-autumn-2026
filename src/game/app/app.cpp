#include "app/app.hpp"
#include <cstdlib>
#include <ctime>

App::App() : render(40), menu() {}

void App::run()
{
    std::srand((unsigned)std::time(nullptr));
    render.init();

    while (!WindowShouldClose())
    {
        MenuAction action = menu.run();

        if (action == MENU_EXIT)
        {
            break;
        }
        if (action == MENU_NEW_FIXED)
        {
            menu.set_message("");
            play<FixedLevels>(FixedLevels("levels/fixed"), nullptr);
        }
        else if (action == MENU_NEW_RANDOM)
        {
            menu.set_message("");
            unsigned seed = (unsigned)std::time(nullptr);
            play<RandomLevels<RANDOM_LEVEL_COUNT>>(
                RandomLevels<RANDOM_LEVEL_COUNT>(seed), nullptr);
        }
        else if (action == MENU_NEW_ENDLESS)
        {
            menu.set_message("");
            unsigned seed = (unsigned)std::time(nullptr);
            play<EndlessLevels>(EndlessLevels(seed), nullptr);
        }
        else if (action == MENU_LOAD)
        {
            SaveData save;
            if (!save_game.read("save.txt", save))
            {
                menu.set_message("Could not read save");
                continue;
            }

            menu.set_message("");
            if (save.mode == MODE_FIXED)
            {
                play<FixedLevels>(FixedLevels("levels/fixed"), &save);
            }
            else if (save.mode == MODE_RANDOM)
            {
                play<RandomLevels<RANDOM_LEVEL_COUNT>>(
                    RandomLevels<RANDOM_LEVEL_COUNT>(save.seed), &save);
            }
            else
            {
                play<EndlessLevels>(EndlessLevels(save.seed), &save);
            }
        }
    }

    render.shutdown();
}

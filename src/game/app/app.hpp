#pragma once
#include "game/game.hpp"
#include "levels/fixed_levels/fixed_levels.hpp"
#include "levels/random_levels/random_levels.hpp"
#include "menu/menu.hpp"
#include "render/render.hpp"
#include "save/save_game.hpp"
#include <utility>

enum
{
    RANDOM_LEVEL_COUNT = 5
};

class App
{
  private:
    Render render;
    Menu menu;

    template <typename Levels>
    void play(Levels levels, SaveData *save)
    {
        Game<Levels> game(std::move(levels));
        GameResult result = game.run(render, save);

        if (result == GameResult::QUIT_TO_MENU)
        {
            SaveData data = game.make_save();
            if (SaveGame::write("save.txt", data))
            {
                menu.set_message("Game saved");
            }
            else
            {
                menu.set_message("Could not save");
            }
        }
        else if (result == GameResult::VICTORY)
        {
            menu.set_message("Victory");
        }
        else if (result == GameResult::DEFEAT)
        {
            menu.set_message("Defeat");
        }
        else if (result == GameResult::ERROR)
        {
            menu.set_message("Could not load levels");
        }
    }

  public:
    App();
    void run();
};

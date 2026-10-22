#pragma once
#include "game/game.hpp"
#include "levels/fixed_levels/fixed_levels.hpp"
#include "levels/random_levels/random_levels.hpp"
#include "menu/menu.hpp"
#include "render/render.hpp"
#include "save/save_game.hpp"

class App
{
  private:
    Render render;
    Menu menu;
    SaveGame save_game;

    template <typename Levels> void play(Levels levels, SaveData *save)
    {
        Game<Levels> game(levels);
        GameResult result = game.run(render, save);

        if (result == GameResult::QUIT_TO_MENU)
        {
            SaveData data = game.make_save();
            if (save_game.write("save.txt", data))
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

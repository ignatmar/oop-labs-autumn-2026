#include "factory.hpp"
#include "game.hpp"
#include "map.hpp"
#include "render.hpp"
#include "robot.hpp"
int main()
{
    std::vector<std::vector<char>> symbols = {{'.', '.', '.', 'M', 'M'},
                                              {'.', '.', '.', '.', '.'},
                                              {'.', 'M', '.', 'S', '.'},
                                              {'.', '.', '.', '.', '.'},
                                              {'M', '.', '.', '.', '.'}};

    Robot player(110, 40, 50, 7, 100, 1, 8, true);

    Robot enemy_1(80, 10, 50, 6, 100, 2, 1, false);
    Robot enemy_2(80, 10, 50, 6, 100, 2, 1, false);
    Robot enemy_3(80, 10, 50, 6, 100, 2, 1, false);

    player.set_coords(1, 1);

    enemy_1.set_coords(10, 10);
    enemy_2.set_coords(20, 20);
    enemy_3.set_coords(22, 5);

    Factory factory_1(23, 6, 3);

    std::vector<Robot *> enemy_robots = {&enemy_1, &enemy_2, &enemy_3};

    std::vector<Factory *> factories = {&factory_1};

    Map map(25, 25, symbols, enemy_robots, factories);

    map.get_cell(1, 1).set_robot(&player);

    map.get_cell(10, 10).set_robot(&enemy_1);
    map.get_cell(20, 20).set_robot(&enemy_2);
    map.get_cell(22, 5).set_robot(&enemy_3);

    map.set_factory(factory_1, 23, 6);

    Game game(map, player);

    Render render(40);

    game.run(render);

    return 0;
}

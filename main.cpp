#include "factory.hpp"
#include "game.hpp"
#include "map.hpp"
#include "render.hpp"
#include "robot.hpp"
#include "world.hpp"
int main()
{
    std::vector<std::vector<char>> symbols = {{'.', '.', '.', 'M', 'M'},
                                              {'.', '.', '.', '.', '.'},
                                              {'.', 'M', '.', 'S', '.'},
                                              {'.', '.', '.', '.', '.'},
                                              {'M', '.', '.', '.', '.'}};

    Robot player(300, 30, 500, 7, 100, 1, 8, true);

    Robot enemy_1(80, 10, 50, 6, 100, 2, 1, false);
    Robot enemy_2(80, 10, 50, 6, 100, 2, 1, false);
    Robot enemy_3(80, 10, 50, 6, 100, 2, 1, false);

    player.set_coords(1, 1);

    enemy_1.set_coords(10, 10);
    enemy_2.set_coords(20, 20);
    enemy_3.set_coords(22, 5);

    Factory factory_1(23, 6, 3);

    Map map(25, 25, symbols);
    World world(map);
    world.set_player(&player);
    world.add_enemy(&enemy_1);
    world.add_enemy(&enemy_2);
    world.add_enemy(&enemy_3);
    world.add_factory(&factory_1);
    world.set_factory(23, 6);

    Game game(world);

    Render render(40);

    game.run(render);

    return 0;
}

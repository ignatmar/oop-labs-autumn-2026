#include "factory/factory.hpp"
#include "robot/robot.hpp"
#include "world/world.hpp"

Factory::Factory(int x, int y, int freq, int rank)
    : x(x), y(y), freq(freq), rank(rank)
{
}

void Factory::spawn(World &world)
{
    if (freq <= 0)
    {
        return;
    }

    int spawn_x[] = {x - 1, x - 1, x + 2, x + 2, x, x + 1, x, x + 1};

    int spawn_y[] = {y, y + 1, y, y + 1, y - 1, y - 1, y + 2, y + 2};

    for (int i = 0; i < 8; i++)
    {
        if (world.get_steps() % freq == 0)
        {
            int new_x = spawn_x[i];
            int new_y = spawn_y[i];

            if (world.check_coords((size_t)new_x, (size_t)new_y))
            {
                Cell &cell = world.get_cell(new_x, new_y);

                if (cell.can_walk() && !cell.if_busy())
                {
                    Robot *robot = new Robot(
                        80 + rank * 40, 10 + rank * 2, 50 + rank * 10, 6,
                        100 + rank * 20, 2, 10, true);
                    robot->set_lvl(rank);
                    robot->set_coords(new_x, new_y);
                    world.add_enemy(robot);
                    return;
                }
            }
        }
    }

    return;
}

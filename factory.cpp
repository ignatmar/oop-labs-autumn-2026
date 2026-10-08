#include "factory.hpp"
#include "map.hpp"
#include "robot.hpp"

Factory::Factory(int x, int y, int freq) : x(x), y(y), freq(freq) {}

void Factory::spawn(Map &map)
{
    int spawn_x[] = {x - 1, x - 1, x + 2, x + 2, x, x + 1, x, x + 1};

    int spawn_y[] = {y, y + 1, y, y + 1, y - 1, y - 1, y + 2, y + 2};

    for (int i = 0; i < 8; i++)
    {
        if (map.get_steps() % freq == 0)
        {
            int new_x = spawn_x[i];
            int new_y = spawn_y[i];

            if (map.check_coords(new_x, new_y))
            {
                Cell &cell = map.get_cell(new_x, new_y);

                if (cell.can_walk() && cell.get_robot() == nullptr)
                {
                    Robot *robot = new Robot(100, 5, 50, 4, 100, 2, 10, true);

                    cell.set_robot(robot);
                    robot->set_coords(new_x, new_y);
                    map.push_enemy(robot);
                    return;
                }
            }
        }
    }

    return;
}

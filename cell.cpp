#include "cell.hpp"

Cell::Cell(char sym) : robot(nullptr), visible(false)
{
    switch (sym)
    {
    case '.':
        surface = GRASS;
        break;
    case 'M':
        surface = MOUNTAIN;
        break;
    case 'S':
        surface = SAND;
        break;
    default:
        surface = GRASS;
    }
}

int Cell::can_walk() { return surface >= 0; }

int Cell::get_surface() { return surface; }

Robot *Cell::get_robot() { return robot; }

void Cell::set_robot(Robot *robot) { this->robot = robot; }

int Cell::if_visible() { return visible; }

void Cell::make_visible() { visible = true; }

void Cell::set_surface(int value) { surface = value; }

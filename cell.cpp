#include "cell.hpp"

Cell::Cell(char sym) : visible(false), has_item(false), has_robot(false)
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

bool Cell::can_walk() { return surface != MOUNTAIN && surface != FACTORY; }

int Cell::get_surface() { return surface; }

bool Cell::if_visible() { return visible; }

void Cell::make_visible() { visible = true; }

void Cell::set_surface(int value) { surface = value; }

bool Cell::if_busy() { return has_robot; }

void Cell::set_busy(bool value) { has_robot = value; }

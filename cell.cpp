#include "cell.hpp"

Cell::Cell(char sym) : robot(nullptr), visible(false), has_item(false)
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

bool Cell::can_walk() { return surface >= 0; }

int Cell::get_surface() { return surface; }

Robot *Cell::get_robot() { return robot; }

void Cell::set_robot(Robot *robot) { this->robot = robot; }

bool Cell::if_visible() { return visible; }

void Cell::make_visible() { visible = true; }

void Cell::set_surface(int value) { surface = value; }

bool Cell::if_has_item() { return has_item; }

ItemType Cell::get_item() { return item; }

void Cell::set_item(ItemType item)
{
    this->item = item;
    has_item = true;
}

void Cell::remove_item() { has_item = false; }

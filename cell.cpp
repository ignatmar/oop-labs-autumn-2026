#include "cell.hpp"
#include "status.hpp"

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

Cell::~Cell()
{
    for (Status *status : statuses)
    {
        delete status;
    }
}

Cell::Cell(Cell &&other)
    : surface(other.surface), visible(other.visible), has_item(other.has_item),
      has_robot(other.has_robot), statuses(other.statuses)
{
    other.statuses.clear();
}

Cell &Cell::operator=(Cell &&other)
{
    if (this == &other)
    {
        return *this;
    }
    for (Status *status : statuses)
    {
        delete status;
    }
    surface = other.surface;
    visible = other.visible;
    has_item = other.has_item;
    has_robot = other.has_robot;
    statuses = other.statuses;
    other.statuses.clear();
    return *this;
}

bool Cell::can_walk() { return surface != MOUNTAIN && surface != FACTORY; }

int Cell::get_surface() { return surface; }

bool Cell::if_visible() { return visible; }

void Cell::make_visible() { visible = true; }

void Cell::set_surface(int value) { surface = value; }

bool Cell::if_busy() { return has_robot; }

void Cell::set_busy(bool value) { has_robot = value; }

void Cell::add_status(Status *status)
{
    if (status != nullptr)
    {
        statuses.push_back(status);
    }
}

Status *Cell::get_status(int index)
{
    if (index < 0 || (size_t)index >= statuses.size())
    {
        return nullptr;
    }
    return statuses[index];
}

Status *Cell::take_status(int index)
{
    if (index < 0 || (size_t)index >= statuses.size())
    {
        return nullptr;
    }
    Status *status = statuses[index];
    statuses.erase(statuses.begin() + index);
    return status;
}

int Cell::get_status_count() { return (int)statuses.size(); }

std::string Cell::get_status_letters()
{
    std::string letters;
    for (Status *status : statuses)
    {
        if (status != nullptr)
        {
            letters += status->get_symbol();
        }
    }
    return letters;
}

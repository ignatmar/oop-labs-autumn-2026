#include "cell/cell.hpp"
#include "status.hpp"

Cell::Cell(char sym) : visible(false), has_robot(false), items{}
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

bool Cell::can_walk() { return surface != MOUNTAIN && surface != FACTORY; }

int Cell::get_surface() { return surface; }

bool Cell::if_visible() { return visible; }

void Cell::make_visible() { visible = true; }

void Cell::set_surface(int value)
{
    surface = value;
    if (!can_walk())
    {
        for (int i = 0; i < ITEM_COUNT; i++)
        {
            items[i] = 0;
        }
        for (Status *status : statuses)
        {
            delete status;
        }
        statuses.clear();
    }
}

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

void Cell::add_item(ItemType type)
{
    if (type >= 0 && type < ITEM_COUNT)
    {
        items[type] = 1;
    }
}

int Cell::get_item_count(ItemType type)
{
    if (type < 0 || type >= ITEM_COUNT)
    {
        return 0;
    }
    return items[type];
}

void Cell::take_item(ItemType type)
{
    if (type >= 0 && type < ITEM_COUNT)
    {
        items[type] = 0;
    }
}

bool Cell::has_items()
{
    for (int i = 0; i < ITEM_COUNT; i++)
    {
        if (items[i] > 0)
        {
            return true;
        }
    }
    return false;
}

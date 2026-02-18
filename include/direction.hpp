#pragma once

enum Direction
{
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
};

inline Direction Opposite(Direction dir)
{
    switch (dir)
    {
    case NORTH:
        return SOUTH;
    case EAST:
        return WEST;
    case SOUTH:
        return NORTH;
    case WEST:
        return EAST;
    }
    return NORTH; // fallback
}
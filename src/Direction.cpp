#include "Direction.h"

#include <stdexcept>

DirectionA opposite(DirectionA dir)
{
    switch (dir)
    {
        case DirectionA::Up:
            return DirectionA::Down;
        case DirectionA::Down:
            return DirectionA::Up;
        case DirectionA::Right:
            return DirectionA::Left;
        case DirectionA::Left:
            return DirectionA::Right;
        case DirectionA::Stay:
            return DirectionA::Stay;
    }
    throw std::runtime_error("Unknown DirectionA");
}

sf::Vector2f toVector(DirectionA dir)
{
    switch (dir)
    {
        case DirectionA::Up:
            return { 0, -1 };
        case DirectionA::Down:
            return { 0, 1 };
        case DirectionA::Right:
            return { 1, 0 };
        case DirectionA::Left:
            return { -1, 0 };
        case DirectionA::Stay:
            return { 0, 0 };
    }
    throw std::runtime_error("Unknown direction");
}

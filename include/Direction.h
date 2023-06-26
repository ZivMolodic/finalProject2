#pragma once

#include <SFML/Graphics.hpp>

enum class DirectionA
{
    Up,
    Down,
    Right,
    Left,
    Stay,
    Max,
};

DirectionA opposite(DirectionA dir);
sf::Vector2f toVector(DirectionA dir);

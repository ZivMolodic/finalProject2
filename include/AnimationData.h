#pragma once

#include "Direction.h"

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <vector>

struct AnimationData
{
    using ListType = std::vector<sf::IntRect>;
    using DataType = std::unordered_map<DirectionA, ListType>;
    DataType m_data;
};

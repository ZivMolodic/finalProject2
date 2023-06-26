#pragma once

#include "Direction.h"
#include "AnimationData.h"

#include <SFML/Graphics.hpp>

class Animation
{
public:
    Animation(const AnimationData& data, DirectionA dir, sf::Shape* sprite, const  std::string& str);

    // Set the direction; the sprite will take the relevant texture rect
    void direction(DirectionA dir);

    // Add more time to the elapsed time; if enough time passed, it
    // updates the sprite to show the next frame in the animation
    void update(sf::Time delta);

private:
    // Update the sprite to take the correct part of the texture,
    // based on current dir and index
    void update();

    const AnimationData& m_data;
    sf::Time m_elapsed = {};
    DirectionA m_dir = DirectionA::Up;
    int m_index = 0;
    sf::Shape* m_sprite;
};

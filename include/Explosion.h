#pragma once
#include "Macros.h"
#include "GameObject.h"
#include <SFML/Graphics.hpp>


class Explosion: public GameObject
{
public:
    Explosion(const sf::Vector2f& position, float radiusLimit)
        : GameObject(0,position, "explosion"), m_radiusLimit(radiusLimit)
    {
        //m_shape.setFillColor(sf::Color::Red);
        //m_shape.setRadius(0);
        //m_shape.setOrigin(0, 0);
        //m_shape.setPosition(position);
    }

    void update() override
    {
        CircleShape* circle = dynamic_cast<CircleShape*>(m_shape.get());
        if(circle)
        {
            if (auto radius = circle->getRadius(); radius < m_radiusLimit) {
                circle->setRadius(radius + 25.f);
                circle->setOrigin(radius, radius);
            }
            else
                setDead();
        }
    }

    bool isFinished() const {
        if (dynamic_cast<CircleShape*>(m_shape.get())->getRadius() >= m_radiusLimit)
            return true;
        else
            return false;
    }

    void draw(sf::RenderWindow* window, const sf::Vector2f& position = sf::Vector2f()) const override {
        window->draw(*m_shape);
    }
    float getLimitRadius() const { return m_radiusLimit; }

private:
    float m_radiusLimit;
    //sf::CircleShape m_shape;
};

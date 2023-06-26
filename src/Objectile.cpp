#include "Objectile.h"


Objectile::Objectile(const sf::Vector2f& position, const sf::Vector2f& destination)
    : DynamicObject(20,position,'*',0.3f,1), m_setteled(false), m_explosion(nullptr)
{
    m_physics->setBodyToRotate(true);
    launch(destination);
    m_timer.restart();
}

void Objectile::handleCollision(const RectangleShape& rec) {
    if (auto update = m_physics->manageCollision(getPosition(), rec); update != sf::Vector2f(0, 0))
        m_shape->setPosition(update);

    if (m_physics->getVelocity() == sf::Vector2f(0, 0))
        m_setteled = true;
}

void Objectile::update()
{
    if (m_explosion)
        m_explosion->update();

    else if (m_timer.getElapsedTime().asSeconds() > 3)
        m_explosion = std::make_shared<Explosion>(getPosition(), 250.f);

    else
    {
        m_physics->update(m_shape.get());

        if (m_physics->getVelocity() == sf::Vector2f(0, 0))
            m_setteled = true;
    }


    //if (auto update = m_physics.manageCollision(m_shape.getPosition(), m_shape.getRadius()); update != sf::Vector2f(0, 0))
    //    m_shape.setPosition(update);

    //if (m_physics.getVelocity() == sf::Vector2f(0,0))
    //    m_setteled = true;
}
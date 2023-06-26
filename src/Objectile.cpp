#include "Objectile.h"


Objectile::Objectile(const sf::Vector2f& position, const sf::Vector2f& destination,
    const std::string& str, float explosionRadius, float timeBound, bool rotation)
    : DynamicObject(15,position,str,0.3f,1), m_explosion(nullptr), m_explosionRadius(explosionRadius),
    m_timeBound(timeBound), m_enableRotation(rotation)
{
    m_physics->setBodyToRotate(true);
    launch(destination);
    m_timer.restart();
}

GuidedMissile::GuidedMissile(const sf::Vector2f& position, const sf::Vector2f& destination)
: Objectile(position, destination, "guided_missile", 150.f, 3.f, true) 
{
    m_physics->setGravity(false);
}

void Grenade::handleCollision(const RectangleShape& rec) 
{
    if (m_timer.getElapsedTime().asSeconds() < 0.3f && m_physics->getVelocity().y < 0)
        return;
    if (auto update = m_physics->manageCollision(getPosition(), rec); update != sf::Vector2f(0, 0))
        m_shape->setPosition(update);

    //if (m_physics->getVelocity() == sf::Vector2f(0, 0))
    //    m_setteled = true;
}

void Tennis::handleCollision(const RectangleShape& rec)
{
    if (m_timer.getElapsedTime().asSeconds() < 0.3f && m_physics->getVelocity().y < 0)
        return;
    if (auto update = m_physics->manageCollision(getPosition(), rec); update != sf::Vector2f(0, 0))
        m_shape->setPosition(update);
}

void Missile::handleCollision(const RectangleShape& rec)
{
    if (m_timer.getElapsedTime().asSeconds() < 0.3f && m_physics->getVelocity().y < 0)
        return;
    if (auto update = m_physics->manageCollision(getPosition(), rec); update != sf::Vector2f(0, 0))
        explode();
}

void GuidedMissile::handleCollision(const RectangleShape& rec)
{
    if (m_timer.getElapsedTime().asSeconds() < 0.3f && m_physics->getVelocity().y < 0)
        return;
    if (auto update = m_physics->manageCollision(getPosition(), rec); update != sf::Vector2f(0, 0))
        explode();
}

void Objectile::update()
{
    if (m_explosion)
        m_explosion->update();

    else if (m_timer.getElapsedTime().asSeconds() > m_timeBound)
        explode();
    else
    {
        m_physics->update(m_shape.get());

        //if (m_physics->getVelocity() == sf::Vector2f(0, 0))
        //    m_setteled = true;
    }


    //if (auto update = m_physics.manageCollision(m_shape.getPosition(), m_shape.getRadius()); update != sf::Vector2f(0, 0))
    //    m_shape.setPosition(update);

    //if (m_physics.getVelocity() == sf::Vector2f(0,0))
    //    m_setteled = true;
}



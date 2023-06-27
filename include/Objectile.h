#pragma once
#include "SFML/Graphics.hpp"
#include "PhysicsBehavior.h"
#include "Resources.h"
#include "Explosion.h"
#include "GameObject.h"


class Objectile: public DynamicObject, public std::enable_shared_from_this<Objectile>
{
public:
    Objectile(const sf::Vector2f& position, const sf::Vector2f& destination, const std::string& str, float explosionRadius, float timeBound, bool rotaion = false);
    virtual ~Objectile() = default;
    virtual void handleCollision(const RectangleShape& rec) = 0;
    virtual void update() override;
    void draw(sf::RenderWindow* window, const sf::Vector2f& position = sf::Vector2f()) const override { if (!m_explosion) window->draw(*m_shape); else m_explosion->draw(window); }
    bool setteled() { if (m_explosion) return m_explosion->isFinished(); return false; }
    std::shared_ptr<GameObject> getObjectile() { if (m_explosion) return m_explosion; else return this->shared_from_this(); }
    sf::Vector2f getVelocity() { return m_physics->getVelocity(); }
    void rotate(float rotation) { if (m_enableRotation) m_physics->rotate(rotation); }
protected:
    sf::Clock m_timer;
    void explode() { m_explosion = std::make_shared<Explosion>(getPosition(), m_explosionRadius);}
private:
    void launch(const Vector2f& velocity) { m_physics->setVelocity(velocity); }
    //bool m_setteled;
    bool m_enableRotation;
    float m_explosionRadius;
    float m_timeBound;
    std::shared_ptr<Explosion> m_explosion;
};


class Grenade : public Objectile
{
public:
    Grenade(const sf::Vector2f& position, const sf::Vector2f& destination) : Objectile(position, destination, "grenade", 150.f, 3.f) {}
    void handleCollision(const RectangleShape& rec) override;
};

class Tennis : public Objectile
{
public:
    Tennis(const sf::Vector2f& position, const sf::Vector2f& destination) : Objectile(position, destination, "tennis", 0, 6.f) {}
    void handleCollision(const RectangleShape& rec) override;
    float getFlightTime() { return m_timer.getElapsedTime().asSeconds(); }
};

class Missile : public Objectile
{
public:
    Missile(const sf::Vector2f& position, const sf::Vector2f& destination) : Objectile(position, destination, "missile", 150.f, 6.f) {}
    void handleCollision(const RectangleShape& rec) override;
};

class GuidedMissile : public Objectile
{
public:
    GuidedMissile(const sf::Vector2f& position, const sf::Vector2f& destination);
    void handleCollision(const RectangleShape& rec) override;
};

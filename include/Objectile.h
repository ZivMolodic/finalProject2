#pragma once
#include "SFML/Graphics.hpp"
#include "PhysicsBehavior.h"
#include "Resources.h"
#include "Explosion.h"
#include "GameObject.h"


class Objectile: public DynamicObject, public std::enable_shared_from_this<Objectile>
{
public:
    Objectile(const sf::Vector2f& position, const sf::Vector2f& destination);
    void handleCollision(const RectangleShape& rec) override;
    void update() override;
    void draw(sf::RenderWindow* window, const sf::Vector2f& position = sf::Vector2f()) const override { if (!m_explosion) window->draw(*m_shape); else m_explosion->draw(window); }
    bool setteled() { if (m_explosion) return m_explosion->isFinished(); return false; }
    std::shared_ptr<GameObject> getObjectile() { if (m_explosion) return m_explosion; else return this->shared_from_this(); }

private:
    void launch(const Vector2f& velocity) { m_physics->setVelocity(velocity); }
    bool m_setteled;
    sf::Clock m_timer;
    std::shared_ptr<Explosion> m_explosion;
};

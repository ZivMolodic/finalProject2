#pragma once
#include <SFML/Graphics.hpp>
#include "Macros.h"
#include <memory>

using namespace sf;


class PhysicsBehavior
{
public:
    PhysicsBehavior(float bounce, float weight);
    virtual ~PhysicsBehavior() = default;
    Vector2f getVelocity() const { return m_velocity; }
    void setWalking(bool walks = true) { m_walking = walks; }
    float getWeight() const { return m_weight; }
    bool isWalking() const { return m_walking; }
    void setJumping(bool jumps = true) { m_jumping = jumps; }
    bool isJumping() const { return m_jumping; }
    void setVelocity(const Vector2f& velocity);
    void setBodyToRotate(bool rotate = true) { m_rotate = rotate; }
    void setBounce(float bounce) { (bounce >= 0) ? m_bounce = bounce : m_bounce = 0; }
    float getBounce() { return m_bounce; }
    void setGravity(bool gravity = true) { m_gravity = gravity; }
    void update(Shape* body);
    void rotate(float rotation);
    virtual sf::Vector2f manageCollision(const sf::Vector2f& position, const RectangleShape& rec = RectangleShape()) = 0;
    
protected:
    void handleHit(const Vector2f& surface);
    float dotProduct(const Vector2f& lhs, const Vector2f& rhs)
    {
        return (lhs.x * rhs.x + lhs.y * rhs.y);
    }
    float norm(const Vector2f& vector)
    {
        return (vector.x * vector.x + vector.y * vector.y);
    }

private:
    bool m_jumping;
    float m_bounce;
    bool m_rotate;
    int m_weight;
    bool m_walking;
    bool m_gravity;
    int m_stabilty;
    Vector2f m_velocity;
    Clock m_timer;
};


class CirclePhysics : public PhysicsBehavior
{
public:
    CirclePhysics(float radius, float bounce, float weight) : PhysicsBehavior(bounce, weight), m_radius(radius){}
    virtual sf::Vector2f manageCollision(const sf::Vector2f& position, const RectangleShape& rec = RectangleShape()) override;

private:
    bool isPointInRotatedRectangle(const sf::Vector2f& point, const sf::RectangleShape& rectangle);
    Vector2f responseVector(const sf::Vector2f& center, const sf::Vector2f& direction, float radius, const RectangleShape& rec);

    float m_radius;
};

class RectanglePhysics : public PhysicsBehavior
{
public:
    RectanglePhysics(const sf::Vector2f& size, float bounce, float weight): PhysicsBehavior(bounce, weight), m_size(size) {}
    virtual sf::Vector2f manageCollision(const sf::Vector2f& position, const RectangleShape& rec = RectangleShape()) override;
    
private:
    sf::Vector2f AABBResponse(const sf::RectangleShape& rec1, const sf::RectangleShape& rec2);

    sf::Vector2f m_size;
};
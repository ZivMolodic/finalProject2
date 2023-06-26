#pragma once
//#include "Macros.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include "PhysicsBehavior.h"

using namespace sf;

class GameObject
{
public:
	//GameObject() {};
	GameObject(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& str);
	GameObject(float radius, const sf::Vector2f& position, const std::string& str);
	virtual ~GameObject() = default;
	sf::FloatRect getGlobalBounds() { return m_shape->getGlobalBounds(); }
	sf::Vector2f getPosition() const { return m_shape->getPosition(); }
	void setPosition(const sf::Vector2f& position) { m_shape->setPosition(position); }
	virtual void draw(RenderWindow* window, const sf::Vector2f& position = sf::Vector2f()) const = 0;
	virtual void update() = 0;
	void setDead() { m_dead = true; }
	bool isDead() { return m_dead; }
	virtual sf::FloatRect getRec() const { return m_shape->getGlobalBounds(); };

	void setBlendMode() { m_shape->setFillColor(sf::Color(255, 255, 255, 128)); }
protected:
	std::unique_ptr<sf::Shape> m_shape;
private:
	bool m_dead;
};


class DynamicObject : public GameObject
{
public:
	DynamicObject(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& str, float bounce, float weight);
	DynamicObject(float radius, const sf::Vector2f& position, const std::string& str, float bounce, float weight);
	virtual ~DynamicObject() = default;
	virtual void update() = 0;
	virtual void handleCollision(const sf::RectangleShape& rec = sf::RectangleShape()) = 0;

protected:
	std::unique_ptr<PhysicsBehavior> m_physics;
};

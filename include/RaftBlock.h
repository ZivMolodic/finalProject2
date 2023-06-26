#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "GameObject.h"

class Explosion;

class RaftBlock: public GameObject
{
public:
	RaftBlock(const Vector2f& size, const sf::Vector2f& position, const std::string& str)
		: GameObject(size, position, str), m_durability(150)
	{}
	void draw(sf::RenderWindow* window, const sf::Vector2f& position = sf::Vector2f()) const override 
	{ 
		window->draw(*m_shape);
		auto rec = this->getRectangle();
		rec.setFillColor(sf::Color::Blue);
		window->draw(rec);
	}
	void update() override {}
	virtual sf::RectangleShape getRectangle() const = 0;
	void handleExplosion(const Explosion& explosion)
	{
		this->setDead();
	}
	
private:
	int m_durability;
};

class DownRaft : public RaftBlock
{
public:
	DownRaft(const Vector2f& position)
		:RaftBlock(Vector2f{ 160, 70 }, position, "raft") {}
	sf::RectangleShape getRectangle() const override
	{
		auto rec = sf::RectangleShape({ 160, 20 });
		rec.setPosition(getPosition().x - 80, getPosition().y - 10);
		return rec;
	}
};

class UpRaft : public RaftBlock
{
public:
	UpRaft(const Vector2f& position)
		:RaftBlock(Vector2f{ 160, 120 }, position, "up_raft1") {}
	sf::RectangleShape getRectangle() const override
	{
		auto rec = sf::RectangleShape({ 160, 20 });
		rec.setPosition(getPosition().x - 80, getPosition().y - 40);
		return rec;
	}
};
#pragma once
#include <SFML/Graphics.hpp>
#include "Resources.h"
#include "GameObject.h"

class RaftBlock: public GameObject
{
public:
	RaftBlock(const Vector2f& size, const sf::Vector2f& position, char c)
		: GameObject(size, position, c), m_durability(150)
	{}
	void draw(sf::RenderWindow* window, const sf::Vector2f& position = sf::Vector2f()) const override 
	{ 
		window->draw(*m_shape);
		/*auto rec = this->getRectangle();
		rec.setFillColor(sf::Color::Blue);
		window->draw(rec);*/
	}
	void update() override {}
	sf::RectangleShape getRectangle() const
	{
		auto rec = sf::RectangleShape({ 160, 20 });
		rec.setPosition(getPosition().x - 80, getPosition().y - 10);
		return rec; 
	}
	
private:
	int m_durability;
};

class DownRaft : public RaftBlock
{
public:
	DownRaft(const Vector2f& position)
		:RaftBlock(Vector2f{ 160, 70 }, position, '#') {}
};

class UpRaft : public RaftBlock
{
public:
	UpRaft(const Vector2f& position)
		:RaftBlock(Vector2f{ 160, 100 }, position, 'u') {}
};
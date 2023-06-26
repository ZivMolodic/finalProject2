#pragma once

#include <memory>
#include "SFML/Graphics.hpp"
#include "Objectile.h"
#include "Resources.h"
#include "RaftBlock.h"
#include "GameObject.h"

class Weapon: public DynamicObject
{
public:
	Weapon();
	bool firing() const { return m_objectile != nullptr; }
	void shot(const sf::Vector2f& destination, const sf::Vector2f& position = sf::Vector2f());
	void update() override;
	void draw(RenderWindow* window, const sf::Vector2f& position) const override;
	std::shared_ptr<GameObject> getObjectile() { if (m_objectile) return m_objectile->getObjectile(); }
	sf::Vector2f getObjectilePosition() { if (m_objectile) return m_objectile->getPosition(); }
	void handleCollision(const RectangleShape& rec) override { if (m_objectile) m_objectile->handleCollision(rec); }

private:
	std::shared_ptr<Objectile> m_objectile;
	int m_ammo;
};

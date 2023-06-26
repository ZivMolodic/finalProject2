#pragma once
#include "GameObject.h"
//#include "Player.h"
#include <SFML/Graphics.hpp>

class Player;
class Computer;
class RaftBlock;

class Board
{
public:
	Board(const sf::Vector2f& playerPosition, const sf::Vector2f& computerPosition, int numOfRaftMen);
	void addObject(GameObject* object);
	void update();
	void draw(RenderWindow* window);
	void play(RenderWindow* window, const sf::Event& event);
	void handleCollisions();
	sf::Vector2f getUserPosition() const;

private:
	std::shared_ptr<Player> m_user;
	std::shared_ptr<Computer> m_computer;
	std::vector<GameObject*> m_objects;
};
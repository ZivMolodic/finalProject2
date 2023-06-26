#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\audio.hpp>
#include "Player.h"

using namespace sf;

class GamePlay
{
public:
	GamePlay();
	void gameLoop(RenderWindow* window);
private:
	void drawTime(unsigned int time, sf::RenderWindow& window);
	void openShot(RenderWindow*, std::shared_ptr<Player> player);
	sf::RectangleShape m_backGround;
	sf::Text m_text;
};

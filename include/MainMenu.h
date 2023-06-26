#pragma once
#include "Macros.h"
#include <SFML/Graphics.hpp>
#include "Button.h"

class MainMenu {
public:
	MainMenu();
	~MainMenu() = default;
	void menuLoop(const sf::Vector2f& size);
private:
	sf::Texture m_logoTexture;
	sf::RectangleShape m_logo;
	std::vector<std::unique_ptr<MainMenuButton>>  m_buttons;
	sf::RectangleShape m_backGround;
};
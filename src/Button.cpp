#include "Button.h"
#include "GamePlay.h"

using namespace sf;

Button::Button(Vector2f position, Vector2f size, char c)
{
	m_picture.setTexture(&Resources::instance().getTexture(c));
	m_picture.setSize(size);
	m_picture.setPosition(position);
	m_picture.setOrigin(m_picture.getSize() / 2.f);
}

void Button::draw(RenderWindow* window, sf::Vector2f cursorLocation)
{ 
	m_picture.setScale(1, 1);

	if (m_picture.getGlobalBounds().contains(cursorLocation))
		m_picture.setScale(1.2, 1.2);

	window->draw(m_picture); 
}


void PlayButton::play(RenderWindow* window)
{
	auto gamePlay = GamePlay();
	gamePlay.gameLoop(window);
}

void HelpButton::play(RenderWindow* window)
{
}
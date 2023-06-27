#include "Weapon.h"

Weapon:: Weapon(const sf::Vector2f& size, const std::string& str)
	: DynamicObject(size, {0,0}, str ,0,0), m_objectile(nullptr)
{
	m_shape->setOrigin({ 0 , 12.5f });
}

void Weapon::shot(const sf::Vector2f& destination, const sf::Vector2f& position)
{
	if (position != sf::Vector2f())
		m_shape->setPosition(position);

	auto launchPosition = m_shape->getTransform().transformPoint({ m_shape->getLocalBounds().left + m_shape->getLocalBounds().width, m_shape->getLocalBounds().top + 7.5f });

	m_objectile = createObjectile(launchPosition, destination);
}

void Weapon::update()
{
	if (m_objectile) 
	{ 
		m_objectile->update(); 
		if (m_objectile->setteled()) 
			m_objectile.reset(); 
	}
}

void Weapon::draw(RenderWindow* window, const sf::Vector2f& position) const
{
	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*window));
	sf::Vector2f direction = mousePosition - position;
	float angle = std::atan2(direction.y, direction.x) * (180 / 3.14f);

	m_shape->setPosition(position);
	m_shape->setRotation(angle);
	window->draw(*m_shape);

	if (m_objectile)
		m_objectile->draw(window);
	else
		drawAim(window, angle, mousePosition);

}

void Weapon::drawAim(sf::RenderWindow* window, float angle, const sf::Vector2f& mousePosition) const
	{
	if(!m_objectile)
	{
		auto velocity = (mousePosition - getPosition());

		float timeStep = 0.1f; // Adjust this value to control the smoothness of the trajectory
		float time = 0.f;
		float velocityX = abs(velocity.x) * cosf(angle * 3.14159f / 180.f);
		float velocityY = abs(velocity.y) * sin(angle * 3.14159f / 180.f);

		std::vector<sf::CircleShape> dots;
		auto potentialPosition = m_shape->getTransform().transformPoint({ m_shape->getLocalBounds().left + m_shape->getLocalBounds().width, m_shape->getLocalBounds().top + 7.5f });
		auto initialPosition = potentialPosition;
		auto circle = sf::CircleShape(3);
		circle.setFillColor(sf::Color::Green);
		circle.setPosition(potentialPosition);
		dots.push_back(circle);

		for (int i = 0; i < 20; ++i)
		{
			time += timeStep;
			potentialPosition.x = initialPosition.x + velocityX * time;
			potentialPosition.y = initialPosition.y + velocityY * time + 10.f * GRAVITY * time * time;

			auto c = sf::CircleShape(3);
			c.setFillColor(sf::Color::Green);
			c.setPosition(potentialPosition);
			dots.push_back(c);
		}

		for (const auto& dot : dots)
			window->draw(dot);
		//while (potentialPosition.y < BACKGROUND_SIZE.y && potentialPosition.x < BACKGROUND_SIZE.x)
		//for (int i = 0; i < 1000; ++i)
		//{
		//	time += 1;
		//	if (rec.getRec().getGlobalBounds().contains(sf::Vector2f(position.x + velocityX * time, position.y + velocityY * time + 25.f * GRAVITY * time * time)))
		//		this->shot(velocity * 0.06f);
		//}
	}
}
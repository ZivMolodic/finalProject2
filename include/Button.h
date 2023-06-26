#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Resources.h"
#include "Macros.h"

using namespace sf;

class Button
{
public:
	Button(Vector2f position, Vector2f size, const std::string& str);
	~Button() = default;
	FloatRect globalBounds() const { return m_picture.getGlobalBounds(); }
	virtual void draw(RenderWindow* window, sf::Vector2f cursorLocation); 
protected:
	RectangleShape m_picture;
};

class VolumeButton : public Button
{
public:
	VolumeButton(Vector2f position)
		:Button(position, Vector2f(60, 60), "volume"), m_playing(true) {}
	~VolumeButton() = default;
	void play(){
		if (m_playing)
		{
			Resources::instance().stopBackGroundMusic();
			m_playing = false;
			m_picture.setTexture(&Resources::instance().getTexture("mute"));
		}
		else
		{
			Resources::instance().playBackGround();
			m_playing = true;
			m_picture.setTexture(&Resources::instance().getTexture("volume"));
		}
	}
private:
	bool m_playing;
};



class MainMenuButton : public Button
{
public:
	MainMenuButton(Vector2f position)
		:Button(position, Vector2f{ 200, 70 }, "menuBlock"), m_text("", Resources::instance().getFont()) {
		m_text.setPosition(position);
		m_text.setCharacterSize(50);
		m_text.setOrigin(m_text.getLocalBounds().width / 2.f, m_text.getLocalBounds().height / 2.f);
		m_text.setFillColor(sf::Color::White);
		m_text.setOutlineThickness(4);
		m_text.setOutlineColor(sf::Color::Black);
	}
	~MainMenuButton() = default;
	void draw(RenderWindow* window, sf::Vector2f cursorLocation) override
	{
		Button::draw(window, cursorLocation);
		window->draw(m_text);
	}
	virtual void play(RenderWindow* window) = 0;
protected:
	sf::Text m_text;
};

class PlayButton : public MainMenuButton
{
public:
	PlayButton(Vector2f position)
		:MainMenuButton(position) 
	{
		m_text.setString("PLAY");
		m_text.setOrigin(Vector2f{ m_text.getGlobalBounds().width / 2.f, m_text.getGlobalBounds().height / 2.f + 10.f });
	}
	~PlayButton() = default;
	void play(RenderWindow* window) override;
};


class HelpButton : public MainMenuButton
{
public:
	HelpButton(Vector2f position)
		:MainMenuButton(position) 
	{
		m_text.setString("HELP");
		m_text.setOrigin(Vector2f{ m_text.getGlobalBounds().width / 2.f, m_text.getGlobalBounds().height / 2.f + 10.f });
	}
	~HelpButton() = default;

	void play(RenderWindow* window) override;
};


class ExitButton : public MainMenuButton
{
public:
	ExitButton(Vector2f position)
		:MainMenuButton(position)
	{
		m_text.setString("EXIT");
		m_text.setOrigin(Vector2f{ m_text.getGlobalBounds().width / 2.f, m_text.getGlobalBounds().height / 2.f + 10.f });
	}
	~ExitButton() = default;
	void play(RenderWindow* window) override { window->close(); };
};


class GameMenuButton : public Button
{
public:
	GameMenuButton(Vector2f position, const std::string& str)
		:Button(position, Vector2f{ 80, 80 }, str) {}
	virtual enum Menu choose() = 0;
};


class UpRaftButton : public GameMenuButton
{
public:
	UpRaftButton(Vector2f position)
		:GameMenuButton(position, "up_raft_button") {}
	enum Menu choose() override { return UP_RAFT; } 
};

class DownRaftButton : public GameMenuButton
{
public:
	DownRaftButton(Vector2f position)
		:GameMenuButton(position, "down_raft_button") {}
	enum Menu choose() override { return DOWN_RAFT; } 
};

class GrenadeButton : public GameMenuButton
{
public:
	GrenadeButton(Vector2f position)
		:GameMenuButton(position, "grenade_button") {}
	enum Menu choose() override { return GRENADE; } 
};

class TennisButton : public GameMenuButton
{
public:
	TennisButton(Vector2f position)
		:GameMenuButton(position, "tennis_button") {}
	enum Menu choose() override { return TENNIS; } 
};

class MissileButton : public GameMenuButton
{
public:
	MissileButton(Vector2f position)
		:GameMenuButton(position, "missile_button") {}
	enum Menu choose() override { return MISSILE; }
};

class GuidedMissileButton : public GameMenuButton
{
public:
	GuidedMissileButton(Vector2f position)
		:GameMenuButton(position, "guided_missile_button") {}
	enum Menu choose() override { return GUIDED_MISSILE; }
};
#pragma once
#include <iostream>
#include <map>
#include "SFML/Graphics.hpp"
#include <SFML/audio.hpp>
#include "AnimationData.h"

class MovingObjects;

//
// this is a static singleton class. it provides and handles
// all game graphics and sounds
//
class Resources
{
public:
	enum Objects
	{
		Missile,
		GuidedMissile,
		MAX
	};
	Resources(const Resources&) = delete;
	void operator = (const Resources&) = delete;
	static Resources& instance();
	sf::Texture &getTexture(const std::string& str);
	sf::Font& getFont() { return m_font;  }
	/*void playMusic(enum Sounds i, int vol = 100);
	void playBackGround();
	void volumeBackGround(int vol);
	void stopBackGroundMusic();*/
	const AnimationData& animationData(Objects object) { return m_animation[object]; }

private:
	Resources();
	void loadGraphics();
//	void loadSounds();
	std::vector<AnimationData> m_animation;

	std::map <std::string, sf::Texture> m_textures;
	sf::Font m_font;
	sf::Texture m_texture;

//	std::vector<sf::SoundBuffer> m_sounds;
//	sf::Music m_backGround;
//	sf::Sound m_sound;
};

#pragma once

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/audio.hpp>
#include "Resources.h"
#include "Macros.h"
#include "RaftBlock.h"
#include "GameObject.h"
#include "MainMenu.h"
#include "Board.h"
#include "Player.h"
#include "Button.h"
#include "GamePlay.h"

int main() 
{
	//Controller controller;
	//controller.run();

	//sf::RenderWindow window(sf::VideoMode(800, 600), "Angry Birds");
	//window.setFramerateLimit(60);

 //   auto player = std::make_shared<Player>(1, Vector2f{ 100, 550 });

 //   bool playerTurn = true;

 //   while (window.isOpen())
 //   {
 //       auto event = sf::Event{};
 //       while( window.pollEvent(event))
 //       {
 //           if (event.type == sf::Event::Closed)
 //               window.close();

 //       }

 //       window.clear();

 //       //update
 //       player->update();
 //       player->draw(&window);

 //       if (playerTurn)
 //       {
 //           player->play(&window, event);
 //           //playerTurn = false;
 //       }
 //       else if (player->isPlaying())
 //           player->play(&window, event);

 //       

 //       window.display();
 //   }
	auto menu = MainMenu();
	menu.menuLoop(WINDOW_SIZE);
}
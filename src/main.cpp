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
	auto menu = MainMenu();
	menu.menuLoop(WINDOW_SIZE);
}
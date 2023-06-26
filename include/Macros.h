#pragma once
#include <SFML/Graphics.hpp>

const sf::Vector2f WINDOW_SIZE = { 900.f, 607.f };
const float SPACE_WITHOUT_BUTTONS = 200.f; //from one side
const sf::Vector2f GRAPH_POS = { 60.f, 40.f }; //from one side
const sf::Vector2f BUTTONS_POSITION = { WINDOW_SIZE.x / 2.f, SPACE_WITHOUT_BUTTONS }; 
const sf::Vector2f GRAPH_SIZE = { WINDOW_SIZE.x - GRAPH_POS.x * 2.f, WINDOW_SIZE.x - GRAPH_POS.x * 2.f };
const auto ORANGE = sf::Color(255, 132, 0) ;
const sf::Vector2f COLOR_SQUARE_SIZE = { 50, 50 };
const sf::Vector2f BACKGROUND_SIZE = { WINDOW_SIZE.x * 3.5f, WINDOW_SIZE.y };
const sf::Vector2f GAME_MENU_BUTTON_SIZE = { 90, 90 };
const float GRAVITY = 0.8f;
const float BOUNCE_FACTOR = 0.7f; // Adjust this value to control the bounce intensity
const int NUMOFPOINTS = 1000;

const sf::Vector2f RAFT_SIZE = { 40, 10 };

enum Menu {TENNIS, GRENADE, MISSILE, GUIDED_MISSILE, UP_RAFT, DOWN_RAFT, NON};

enum class Direction {
    Up,
    Down,
    Left,
    Right, 
    NA
};



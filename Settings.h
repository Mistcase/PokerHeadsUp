#pragma once
#ifndef _SETTINGS_DEFINED_
#define _SETTINGS_DEFINED_

#include <SFML/System.hpp>

inline sf::Vector2f MultiplyVector(sf::Vector2f vector, float multiplier)
{
	return sf::Vector2f(vector.x * multiplier, vector.y * multiplier);
}

const sf::Vector2f APPLICATION_WINDOW_SIZE = sf::Vector2f(800, 600);
const sf::Vector2f GAMESTATE_BUTTON_SIZE = sf::Vector2f(120, 40);
constexpr size_t PLAYER_NAME_MAX_SIZE = 32;
constexpr size_t PLAYERS_COUNT = 2;

#endif

#pragma once
#ifndef _SETTINGS_DEFINED_
#define _SETTINGS_DEFINED_

#include "Types.h"

inline sf::Vector2f MultiplyVector(sf::Vector2f vector, float multiplier)
{
	return sf::Vector2f(vector.x * multiplier, vector.y * multiplier);
}

const Vector2f APPLICATION_WINDOW_SIZE = Vector2f(800, 600);
const Vector2f GAMESTATE_BUTTON_SIZE = Vector2f(120, 40);
constexpr size_t PLAYER_NAME_MAX_SIZE = 32;
constexpr size_t PLAYERS_COUNT = 2;

#include <Address.hpp>
const netboost::Address SERVER_ADDRESS = netboost::Address("127.0.0.1:12345");
const netboost::Address SERVER_ADDRESS_FAKE_ADDRESS = netboost::Address(""); //TEST FAILED CONNECTION

#endif

#pragma once
#ifndef _PLAYER_DEFINED_
#define  _PLAYER_DEFINED_

#include <SFML/Graphics.hpp>

#include "Settings.h"
#include "GameFont.h"

namespace table_slots
{
	enum Value
	{
		UNKNOWN,
		TOP,
		BOTTOM
	};
}

class Player : public sf::Drawable
{
public:
	const unsigned int PLAYER_START_BALANCE = 1000;

public:
	Player();
	void setNickname(const sf::String& name);
	void setPlayerSlot(table_slots::Value slot);

	const sf::String& getNickname();

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	//Player data
	sf::String name;
	unsigned int balance = PLAYER_START_BALANCE;
	table_slots::Value playerSlot = table_slots::UNKNOWN;

	//Graphics data
	sf::Text playerString;
	sf::Texture tChips;
	sf::RectangleShape chipsSprite;
};

#endif
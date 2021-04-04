#pragma once
#ifndef _PLAYER_DEFINED_
#define  _PLAYER_DEFINED_

#include <SFML/Graphics.hpp>

#include "Settings.h"
#include "Fonts.h"
#include "Cards.h"
#include "TableSlots.h"

class Player : public sf::Drawable
{
public:
	const unsigned int PLAYER_START_BALANCE = 1000;

public:
	enum PlayerDescision
	{
		UNKNOWN,
		CHECK,
		CALL,
		BET,
		RAISE,
		FOLD
	};

public:
	Player();
	void setNickname(const sf::String& name);
	void setPlayerSlot(table_slots::Value slot);

	void setPlayerDescision(PlayerDescision desc);
	void setBalance(Balance balance);
	Balance makeBet(Balance betValue);
	void zeroCurrentBet();

	const sf::String& getNickname() const;
	Balance getBalance() const;
	Balance getCurrentBet() const;
	PlayerDescision getAction() const;

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	//Player data
	sf::String name;
	Balance balance = PLAYER_START_BALANCE, currentBet = 0;
	table_slots::Value playerSlot = table_slots::UNKNOWN;
	PlayerDescision action = UNKNOWN;

	//Graphics data
	sf::Text playerString;
	sf::Texture tChips;
	sf::RectangleShape chipsSprite;
};

#endif
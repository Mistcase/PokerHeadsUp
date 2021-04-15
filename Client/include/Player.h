#pragma once
#ifndef _PLAYER_DEFINED_
#define  _PLAYER_DEFINED_

#include <SFML/Graphics.hpp>

#include "Types.h"
#include "Settings.h"
#include "Fonts.h"
#include "TableSlots.h"
#include "Resources.h"

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

	void setBalance(Balance balance);
	void setBet(Bet betValue);
	Bet makeBet(Bet betValue);

	const String& getNickname() const;
	Balance getBalance() const;
	Balance getCurrentBet() const;

	void update();

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	//Player data
	String name;
	Balance balance = PLAYER_START_BALANCE, currentBet = 0;
	table_slots::Value playerSlot = table_slots::UNKNOWN;
	PlayerDescision action = UNKNOWN;

	//Graphics data
	Text playerString;
	Texture tChips;
	RectangleShape chipsSprite;

	//Current Bet
	Text g_betValue;
};

#endif
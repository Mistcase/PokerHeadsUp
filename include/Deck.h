#pragma once
#ifndef _DECK_DEFINED_
#define _DECK_DEFINED_

#include <utility>
#include <ctime>
#include <vector>

#include "Cards.h"

constexpr unsigned int FULL_DECK_CARDS_NUMBER = 52;

class Deck
{
public:
	Deck();

	void shuffle();
	Card getTopCard();
	Card removeTopCard();

private:
	std::vector<Card> cards;
};
#endif
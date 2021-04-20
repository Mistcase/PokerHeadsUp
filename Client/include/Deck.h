#pragma once
#ifndef _DECK_H_
#define _DECK_H_

#include "Types.h"
#include "Settings.h"
#include "Resources.h"
#include "Notifications.h"

class Deck
{
public:
	static const size_t DECK_CARDS_COUNT = 52;

	enum CardsSuitsGraphicsId : int
	{
		GI_CLUBS,
		GI_SPADES,
		GI_HEARTS,
		GI_DIAMONDS,
		GI_SUIT_UNKNOWN,
		GI_SUITS_COUNT
	};
	enum CardsValuesGraphicsId : int
	{
		GI_ACE,
		GI_TWO,
		GI_THREE,
		GI_FOUR,
		GI_FIVE,
		GI_SIX,
		GI_SEVEN,
		GI_EIGHT,
		GI_NINE,
		GI_TEN,
		GI_JACK,
		GI_QUEEN,
		GI_KING,
		GI_ACE_TOP,
		GI_BACK_CARD,
	};

public:
	static void setCard(RectangleShape* shape, CardsValuesGraphicsId val, CardsSuitsGraphicsId suit);

private:
	static Texture cardsTexture, backCardTexture;
};

typedef pair<Deck::CardsValuesGraphicsId, Deck::CardsSuitsGraphicsId> Card;

#endif
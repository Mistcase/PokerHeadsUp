#include "Deck.h"

void Deck::setCard(RectangleShape * shape, CardsValuesGraphicsId val, CardsSuitsGraphicsId suit)
{
	static bool firstCall = true;

	if (firstCall)
	{
		cardsTexture.loadFromFile(Resources::path("cards.png"));
		backCardTexture.loadFromFile(Resources::path("card_back.png"));
		firstCall = false;
	}

	if (val == GI_BACK_CARD)
	{
		shape->setTexture(&backCardTexture, true);
	}
	else
	{
		shape->setTexture(&cardsTexture);
		shape->setTextureRect(IntRect(GAMESTATE_CARD_SIZE.x * val, GAMESTATE_CARD_SIZE.y * suit, GAMESTATE_CARD_SIZE.x, GAMESTATE_CARD_SIZE.y));
	}
}

Texture Deck::cardsTexture, Deck::backCardTexture;
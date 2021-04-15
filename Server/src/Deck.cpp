#include "Deck.h"
#include <stack>

Deck::Deck()
{
	cards.resize(FULL_DECK_CARDS_NUMBER);

	int counter = 0;
	for (int val = CardValue::TWO; val <= CardValue::ACE; val++)
	{
		for (int suit = CardSuit::CARD_SUIT_MIN_VALUE; suit <= CardSuit::DIFFERENT_CARD_SUITS_NUMBER; suit++)
		{
			cards[counter++] = Card(static_cast<CardValue>(val), static_cast<CardSuit>(suit));
		}
	}
}

void Deck::shuffle()
{
	for (int i = 0; i < FULL_DECK_CARDS_NUMBER; i++)
	{
		std::swap(cards[i], cards[std::rand() % FULL_DECK_CARDS_NUMBER]);
	}
}

Card Deck::getTopCard()
{
	return cards.front();
}

Card Deck::removeTopCard()
{
	Card topCard = cards.front();
	cards.erase(cards.begin());
	
	return topCard;
}

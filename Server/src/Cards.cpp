#include "Cards.h"

Card::Card(CardValue value, CardSuit suit)
{
	this->value = value;
	this->suit = suit;
}

CardValue Card::getValue()
{
	return value;
}

CardSuit Card::getSuit()
{
	return suit;
}

bool Card::isEaqualValue(const Card& other)
{
	return value == other.value;
}

bool Card::operator==(const Card& other)
{
	return value == other.value && suit == other.suit;
}

bool Card::operator>(const Card& other)
{
	return value > other.value;
}

ostream& operator<<(ostream& os, const Card& card)
{
	os << ENUM_VALUES_STR[card.value] << " " << ENUM_SUITS_STR[card.suit];

	return os;
}

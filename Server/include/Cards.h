#pragma once
#ifndef _CARDS_H_
#define _CARDS_H_

#include "Types.h"

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <array>

using std::ostream;
using std::pair;
using std::array;

enum CardValue : int
{
	VALUE_UNKNOWN,
	TWO = 2,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,
	ACE,
	DIFFERENT_CARD_VALUES_NUMBER = 13
};

static const AnsiString ENUM_VALUES_STR[] = { "VALUE_UNKNOW", "VALUE_UNKNOWN", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING", "ACE"};

static const AnsiString ENUM_SUITS_STR[] = { "SUIT_UNKNOWN", "HEARS", "DIAMONDS", "CLUBS", "SPADES"};

enum CardSuit
{
	SUIT_UNKNOWN,
	HEARTS,
	DIAMONDS,
	CLUBS,
	SPADES,

	CARD_SUIT_MIN_VALUE = 1,
	DIFFERENT_CARD_SUITS_NUMBER = 4
};

class Card
{
public:
	Card() = default;
	Card(CardValue value, CardSuit suit);

	CardValue getValue();
	CardSuit getSuit();

	bool isEaqualValue(const Card& other);

	bool operator== (const Card& other);
	bool operator> (const Card& other);
	friend ostream& operator<<(ostream& os, const Card& card);

private:
	CardValue value = VALUE_UNKNOWN;
	CardSuit suit = SUIT_UNKNOWN;
};

typedef pair<Card, Card> PlayerHand;
typedef array<Card, 5> BoardCards;

#define CardsContainer(containerType) containerType<Card>

#endif
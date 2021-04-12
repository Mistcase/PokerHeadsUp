#pragma once
#ifndef _CARDS_H_
#define _CARDS_H_

#include <string>
#include <map>
#include <iostream>

using std::ostream;

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

static const std::string ENUM_VALUES_STR[] = { "VALUE_UNKNOW", "VALUE_UNKNOWN", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING", "ACE"};

static const std::string ENUM_SUITS_STR[] = { "SUIT_UNKNOWN", "HEARS", "DIAMONDS", "CLUBS", "SPADES"};

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

#endif
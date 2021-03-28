#pragma once
#ifndef _COMBINATION_IDENTIFIER_INCLUDED_
#define _COMBINATION_IDENTIFIER_INCLUDED_

#include <algorithm>

#include "Types.h"
#include "Cards.h"

using std::find;
using std::find_if;
using std::sort;

namespace poker_combs
{
	enum Combinations
	{
		COMBINATION_NONE,
		HIGH_CARD,
		PAIR,
		TWO_PAIRS,
		THREE_OF_KIND,
		STRAIGHT,
		FLUSH,
		FULL_HOUSE,
		FOUR_OF_KIND,
		STRAIGHT_FLUSH,
		ROYALE_FLUSH
	};
}

//Uncorrect class name
class CombinationIdentifier
{
public:
	struct Combination
	{
		poker_combs::Combinations combValue = poker_combs::COMBINATION_NONE;
		vector<Card> combCards, kickers;
	};

	struct FlushState
	{
		CardSuit suit;
		bool exists = false;

		vector<Card> allFlushCards;
	};
	
	struct RCState
	{
		vector<CardsContainer(vector)> sets, pairs;
	};

	CombinationIdentifier(const PlayerHand& hand, const BoardCards& board);
	Combination identify();

private:
	void determinateRCState();
	void findKickers(Combination& resultCombination);

	bool IsStraightFlush();
	bool IsFourOfKind();
	bool IsFullHouse();
	bool IsFlush();
	bool IsStraight();
	bool IsThreeOfKind();
	bool IsTwoPairs();
	bool IsPair();

private:
	Combination identificationResult;
	Combination subCombination; //Used for Four of kind of fullHouse

	array<Card, 7> allCards;
	FlushState flush;
	RCState rcState;

private:
	void initStraights();
	static array<array<CardValue, STRAIGHT_CARDS_COUNT>, STRAIGHT_POSSIBLE_VARIATIONS_COUNT> STRAIGHTS;
};

#endif

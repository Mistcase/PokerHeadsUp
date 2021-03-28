#include "CombinationIdentifier.h"

CombinationIdentifier::CombinationIdentifier(const PlayerHand& hand, const BoardCards& board)
{
	allCards = { hand.first, hand.second, board[0], board[1], board[2], board[3], board[4] };
	sort(allCards.begin(), allCards.end(), [](Card& first, Card& second) { return first > second; });
}

CombinationIdentifier::Combination CombinationIdentifier::identify()
{
	initStraights();
	determinateRCState(); //Find all repeated cards

	bool flagFourOfKind = IsFourOfKind(),
		flagFullHouse = IsFullHouse();

	if (IsFlush())
	{
		if (IsStraightFlush())
		{
			return identificationResult;
		}

		if (flagFourOfKind || flagFullHouse)
		{
			return subCombination;
		}

		return identificationResult;
	}

	if (flagFourOfKind || flagFullHouse)
	{
		return subCombination;
	}

	if (IsStraight())
		return identificationResult;

	if (IsThreeOfKind())
		return identificationResult;

	if (IsTwoPairs())
		return identificationResult;

	if (IsPair())
		return identificationResult;

	identificationResult.combValue = poker_combs::HIGH_CARD;
	findKickers(identificationResult);

	return identificationResult;
}

void CombinationIdentifier::determinateRCState()
{
	vector<Card> tempVector;
	for (int cardValue = CardValue::ACE; cardValue >= CardValue::TWO; cardValue--)
	{
		Card cards[] = { Card(static_cast<CardValue>(cardValue), CardSuit::CLUBS),
						Card(static_cast<CardValue>(cardValue), CardSuit::DIAMONDS),
						Card(static_cast<CardValue>(cardValue), CardSuit::HEARTS),
						Card(static_cast<CardValue>(cardValue), CardSuit::SPADES) };

		for (auto card : cards)
		{
			if (find(allCards.begin(), allCards.end(), card) != allCards.end())
			{
				tempVector.emplace_back(card);
			}
		}

		if (tempVector.size() == 2)
		{
			rcState.pairs.emplace_back(tempVector);
		}
		else if (tempVector.size() == 3)
		{
			rcState.sets.emplace_back(tempVector);
		}

		tempVector.resize(0);
	}
}

void CombinationIdentifier::findKickers(Combination& resultCombination)
{
	size_t kickersCount = 5 - resultCombination.combCards.size();

	if (!kickersCount)
		return;

	//As cards are sorted
	for (auto& card : allCards)
	{
		if (find(resultCombination.combCards.begin(), resultCombination.combCards.end(), card) == resultCombination.combCards.end())
		{
			resultCombination.kickers.emplace_back(card);

			if (--kickersCount == 0)
				break;
		}
	}
}



bool CombinationIdentifier::IsStraight()
{
	for (auto& straight : STRAIGHTS)
	{
		bool flag = true;
		for (auto& cardValue : straight)
		{
			auto straightCard = find_if(allCards.begin(), allCards.end(), [&](Card& card) {return card.getValue() == cardValue; });
			if (straightCard != allCards.end())
			{
				identificationResult.combCards.push_back(*straightCard);
			}
			else
			{
				flag = false;
				break;
			}
		}

		if (flag)
		{
			identificationResult.combValue = poker_combs::STRAIGHT;
			return true;
		}
		identificationResult.combCards.resize(0);
	}

	return false;
}

bool CombinationIdentifier::IsStraightFlush()
{
	if (!flush.exists)
		return false;

	Combination sfCombination;

	bool straightFlushFlag;
	for (auto& straight : STRAIGHTS)
	{
		straightFlushFlag = true;
		for (auto& cardValue : straight)
		{
			auto straightCard = find_if(flush.allFlushCards.begin(), flush.allFlushCards.end(), [&](Card& card) {return card.getValue() == cardValue; });
			if (straightCard != flush.allFlushCards.end())
			{
				sfCombination.combCards.push_back(*straightCard);
			}
			else
			{
				straightFlushFlag = false;
				break;
			}
		}

		if (straightFlushFlag)
		{
			sfCombination.combValue = (sfCombination.combCards[0].getValue() == ACE) ? poker_combs::ROYALE_FLUSH :
				poker_combs::STRAIGHT_FLUSH;
			identificationResult = sfCombination;
			return true;
		}
		sfCombination.combCards.resize(0);
	}

	return false;
}

bool CombinationIdentifier::IsFlush()
{
	int scCount[4] = { 0, 0, 0, 0 };

	for (auto& card : allCards)
	{
		if (++scCount[card.getSuit() - CardSuit::CARD_SUIT_MIN_VALUE] == 5)
		{
			flush.exists = true;
			flush.suit = card.getSuit();
			break;
		}
	}

	if (flush.exists)
	{
		for (int cardValue = CardValue::ACE; cardValue >= CardValue::TWO; cardValue--)
		{
			Card card(static_cast<CardValue>(cardValue), flush.suit);
			if (find(allCards.begin(), allCards.end(), card) != allCards.end())
				identificationResult.combCards.push_back(card);

			if (identificationResult.combCards.size() >= 5)
			{
				for (auto& card : allCards)
				{
					if (card.getSuit() == flush.suit)
						flush.allFlushCards.emplace_back(card);
				}

				identificationResult.combValue = poker_combs::FLUSH;
				break;
			}
		}
	}

	return flush.exists;
}

bool CombinationIdentifier::IsFourOfKind()
{
	for (int cardValue = CardValue::ACE; cardValue >= CardValue::TWO; cardValue--)
	{
		for (auto& card : allCards)
		{
			if (card.getValue() == cardValue)
			{
				subCombination.combCards.emplace_back(card);
			}
		}

		if (subCombination.combCards.size() == 4)
		{
			subCombination.combValue = poker_combs::FOUR_OF_KIND;
			findKickers(subCombination);
			return true;
		}

		subCombination.combCards.resize(0);
	}

	return false;
}

bool CombinationIdentifier::IsFullHouse()
{
	if (subCombination.combValue == poker_combs::FOUR_OF_KIND)
		return false;

	if (rcState.pairs.size() > 0 &&
		rcState.sets.size() > 0)
	{
		subCombination.combValue = poker_combs::FULL_HOUSE;
		for (auto& card : rcState.sets[0])
			subCombination.combCards.emplace_back(card);
		for (auto& card : rcState.pairs[0])
			subCombination.combCards.emplace_back(card);
		return true;
	}

	return false;
}

bool CombinationIdentifier::IsThreeOfKind()
{
	if (rcState.sets.size() == 0)
		return false;

	identificationResult.combValue = poker_combs::THREE_OF_KIND;
	for (auto& card : rcState.sets.front())
	{
		identificationResult.combCards.emplace_back(card);
	}
	findKickers(identificationResult);

	return true;
}

bool CombinationIdentifier::IsTwoPairs()
{
	if (rcState.pairs.size() >= 2)
	{
		identificationResult.combValue = poker_combs::TWO_PAIRS;
		identificationResult.combCards.emplace_back(rcState.pairs[0].at(0));
		identificationResult.combCards.emplace_back(rcState.pairs[0].at(1));
		identificationResult.combCards.emplace_back(rcState.pairs[1].at(0));
		identificationResult.combCards.emplace_back(rcState.pairs[1].at(1));
		findKickers(identificationResult);
		return true;
	}
	return false;
}

bool CombinationIdentifier::IsPair()
{
	if (rcState.pairs.size() > 0)
	{
		identificationResult.combValue = poker_combs::PAIR;
		identificationResult.combCards.emplace_back(rcState.pairs[0].at(0));
		identificationResult.combCards.emplace_back(rcState.pairs[0].at(1));
		findKickers(identificationResult);
		return true;
	}

	return false;
}


void CombinationIdentifier::initStraights()
{
	static bool initialized = false;
	if (initialized)
		return;

	CardValue highStraightCardValue = ACE;
	for (int i = 0; i < STRAIGHT_POSSIBLE_VARIATIONS_COUNT; i++)
	{
		for (int j = 0; j < STRAIGHT_CARDS_COUNT; j++)
		{
			STRAIGHTS[i][j] = static_cast<CardValue>(highStraightCardValue - j);
		}
		highStraightCardValue = static_cast<CardValue>(static_cast<int>(highStraightCardValue) - 1);
	}

	STRAIGHTS[STRAIGHT_POSSIBLE_VARIATIONS_COUNT - 1][STRAIGHT_CARDS_COUNT - 1] = ACE;
	initialized = true;
}

array<array<CardValue, STRAIGHT_CARDS_COUNT>, STRAIGHT_POSSIBLE_VARIATIONS_COUNT> CombinationIdentifier::STRAIGHTS;
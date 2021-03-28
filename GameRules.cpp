#include "GameRules.h"

#include <iostream>
using std::cout;
using std::endl;

//void InitGameRules()
//{
//	using namespace poker_combs;
//
//	typedef pair<Combinations, size_t> CombinationPair;
//
//	KICKERS_COUNT.insert(CombinationPair(HIGH_CARD, 5));
//	KICKERS_COUNT.insert(CombinationPair(PAIR, 3));
//	KICKERS_COUNT.insert(CombinationPair(TWO_PAIRS, 1));
//	KICKERS_COUNT.insert(CombinationPair(THREE_OF_KIND, 2));
//	KICKERS_COUNT.insert(CombinationPair(STRAIGHT, 0));
//	KICKERS_COUNT.insert(CombinationPair(FLUSH, 0));
//	KICKERS_COUNT.insert(CombinationPair(FULL_HOUSE, 0));
//	KICKERS_COUNT.insert(CombinationPair(FOUR_OF_KIND, 1));
//	KICKERS_COUNT.insert(CombinationPair(STRAIGHT_FLUSH, 0));
//	KICKERS_COUNT.insert(CombinationPair(ROYALE_FLUSH, 0));
//}
//
//Combination IdentifyCombination(const PlayerHand& playerCards, const BoardCards& board)
//{
//	Combination result;
//	array<Card, 7> allCards = { playerCards.first, playerCards.second, board[0], board[1], board[2], board[3], board[4] };
//
//	//Sort for kickers
//	std::sort(allCards.begin(), allCards.end(), [&](Card& first, Card& second)
//	{
//		return first.getValue() > second.getValue();
//	});
//
//	CardSuit flushSuit = CardSuit::SUIT_UNKNOWN;
//	int scCount[4] = { 0, 0, 0, 0 };
//
//	bool flushFlag = false;
//	for (auto& card : allCards)
//	{
//		if (++scCount[card.getSuit() - CardSuit::CARD_SUIT_MIN_VALUE] >= 5)
//		{
//			flushFlag = true;
//			flushSuit = card.getSuit();
//			break;
//		}
//	}
//
//	//В теории
//	//Только одно возможное каре
//	//2 сета
//	//До 3-х пар
//
//	//Four of kind
//	//Three of kind
//	//All pairs
//	bool flagFourOfKind = false,
//		flagThreeOfKind = false,
//		flagPair = false;
//	vector<Card> threeOfKindValues, pairValues;
//	Card fourOfKindValue;
//	{
//		for (int cardValue = CardValue::ACE; cardValue >= CardValue::TWO; cardValue--)
//		{
//			bool flagEnd = false;
//			for (auto& card : allCards)
//			{
//				if (card.getValue() == cardValue)
//				{
//					result.combCards.emplace_back(card);
//				}
//			}
//
//			switch (result.combCards.size())
//			{
//			case 4:
//				//Only one time is possible
//				fourOfKindValue = result.combCards.front();
//				flagFourOfKind = true;
//				flagEnd = true;
//				break;
//
//			case 3:
//				threeOfKindValues.emplace_back(result.combCards.front());
//				flagThreeOfKind = true;
//				break;
//
//			case 2:
//				pairValues.emplace_back(result.combCards.front());
//				flagPair = true;
//				break;
//			}
//
//			if (flagEnd)
//				break;
//
//			result.combCards.resize(0);
//		}
//	}
//
//
//	Combination intermidiate;
//	//Four of kind
//	if (flagFourOfKind)
//	{
//		intermidiate.combValue = poker_combs::FOUR_OF_KIND;
//		for (int i = 0; i < 4; i++)
//			intermidiate.combCards.emplace_back(fourOfKindValue);
//	}
//
//	//Full house
//	if (flagThreeOfKind && flagPair)
//	{
//		intermidiate.combValue = poker_combs::FULL_HOUSE;
//		for (int i = 0; i < 3; i++)
//			intermidiate.combCards.emplace_back(threeOfKindValues[0]);
//		for (int i = 0; i < 2; i++)
//			intermidiate.combCards.emplace_back(pairValues[0]);
//	}
//
//	if (flushFlag)
//	{
//		//Straight-Flush
//		{
//			vector<Card> flushCards;
//			for (auto& card : allCards)
//			{
//				if (card.getSuit() == flushSuit)
//					flushCards.push_back(card);
//			}
//
//			bool straightFlushFlag;
//			CardValue sfHigherCardValue;
//
//			for (auto& straight : STRAIGHTS)
//			{
//				straightFlushFlag = true;
//				sfHigherCardValue = straight.at(0);
//				for (auto cardValue : straight)
//				{
//					bool founded = false;
//					for (auto& flushCard : flushCards)
//					{
//						if (flushCard.getValue() == cardValue)
//						{
//							founded = true;
//							result.combCards.push_back(flushCard);
//							break;
//						}
//					}
//					if (!founded)
//					{
//						straightFlushFlag = false;
//						break;
//					}
//				}
//
//				if (straightFlushFlag)
//				{
//					result.combValue = result.combCards[0].getValue() == ACE ? poker_combs::ROYALE_FLUSH : poker_combs::STRAIGHT_FLUSH;
//					goto end;
//				}
//				sfHigherCardValue = CardValue::VALUE_UNKNOWN;
//				result.combCards.resize(0);
//			}
//		}
//
//		//If no straight-flush
//		if (flagFourOfKind || flagThreeOfKind)
//		{
//			result = intermidiate;
//			goto end;
//		}
//
//		//Flush
//		{
//			for (int cardValue = CardValue::ACE; cardValue >= CardValue::TWO; cardValue--)
//			{
//				Card card(static_cast<CardValue>(cardValue), flushSuit);
//				if (std::find(allCards.begin(), allCards.end(), card) != allCards.end())
//				{
//					result.combCards.push_back(card);
//				}
//				if (result.combCards.size() >= 5)
//				{
//					result.combValue = poker_combs::FLUSH;
//					goto end;
//				}
//			}
//		}
//
//	}
//	else if (flagFourOfKind || flagThreeOfKind)
//	{
//		result = intermidiate;
//		goto end;
//	}
//
//
//	//Straight
//	for (auto& straight : STRAIGHTS)
//	{
//		for (auto& sValue : straight)
//		{
//			bool founded = false;
//			for (auto& bCard : allCards)
//			{
//				if (bCard.getValue() == sValue)
//				{
//					founded = true;
//					result.combCards.emplace_back(bCard);
//					break;
//				}
//			}
//			if (!founded)
//			{
//				result.combCards.resize(0);
//				break;
//			}
//		}
//
//		if (result.combCards.size() >= 5)
//		{
//			result.combValue = poker_combs::STRAIGHT;
//			goto end;
//		}
//	}
//
//	//Set
//	if (flagThreeOfKind)
//	{
//		result.combValue = poker_combs::THREE_OF_KIND;
//		for (int i = 0; i < 3; i++)
//		{
//			result.combCards.emplace_back(threeOfKindValues.front());
//		}
//
//		goto end;
//	}
//
//	//Pair and two pairs
//	if (flagPair)
//	{
//		for (int i = 0; i < 2; i++)
//			result.combCards.emplace_back(pairValues[0]);
//
//		if (pairValues.size() >= 2)
//		{
//			for (int i = 0; i < 2; i++)
//				result.combCards.emplace_back(pairValues[1]);
//
//			result.combValue = poker_combs::TWO_PAIRS;
//		}
//		else
//		{
//			result.combValue = poker_combs::PAIR;
//		}
//	}
//
//end:
//	return result;
//}
//
//
////map<poker_combs::Combinations, size_t> KICKERS_COUNT;
//array<array<CardValue, STRAIGHT_CARDS_COUNT>, STRAIGHT_POSSIBLE_VARIATIONS_COUNT> STRAIGHTS;
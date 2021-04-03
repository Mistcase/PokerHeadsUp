#pragma once
#ifndef _POKER_HAND_INCLUDED_
#define _POKER_HAND_INCLUDED_

#include "RingQueue.h"
#include "Observer.h"
#include "CombinationIdentifier.h"

class PokerHand : public Observerable, public Observer
{
private:
	enum Stage
	{
		STAGE_PREFLOP,
		STAGE_FLOP,
		STAGE_TURN,
		STAGE_RIVER
	};

public:
	PokerHand() = default;
	PokerHand(Observerable* gameState, const RingPlayersQueue& players);
	void play();

	void notifyObservers(const ObsMessageString& message = "") override;
	void handleEvent(const ObsMessage& message) override;

private:
	RingPlayersQueue players;
	const Player* button;
	const Player* bb;

	Balance smallBlind = 10;
};

#endif 
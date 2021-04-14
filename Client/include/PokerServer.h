#pragma once
#ifndef _POKER_SERVER_INCLUDED_
#define _POKER_SERVER_INCLUDED_

#include <iostream>
#include <thread>

#include "RingQueue.h"
#include "Observer.h"
#include "CombinationIdentifier.h"
#include "Notifications.h"

class PokerServer : public Observerable, public Observer
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
	PokerServer() = default;
	PokerServer(const RingPlayersQueue& players);
	void start();

	void notifyObservers(const EventMessageString& message = "") override;
	void handleEvent(const EventMessage& message) override;

	const Player* getCurrentPlayer() const;
	int getPlayersCount();

private:
	void waitForExternalFeedback();
	bool flagHaveExternalMessage = false;

private:
	RingPlayersQueue players;
	const Player* button = nullptr;
	const Player* bb = nullptr;

	Balance smallBlind = 10, currentMaxBet = 0;
};

#endif 
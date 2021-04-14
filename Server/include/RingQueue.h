#pragma once
#ifndef _RING_QUEUE_INCLUDED
#define _RING_QUEUE_INCLUDED

#include <queue>

#include "Player.h"

class RingPlayersQueue
{
public:
	void push(Player* player);
	void next();
	void pop();
	Player* front() const;

	size_t size() const;
	bool haveEaqualsBets();

private:
	std::queue<Player*> playersQueue;
};

#endif
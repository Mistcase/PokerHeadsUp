#include "RingQueue.h"

void RingPlayersQueue::push(Player* player)
{
	playersQueue.push(player);
}

void RingPlayersQueue::next()
{
	playersQueue.push(playersQueue.front());
	playersQueue.pop();
}

void RingPlayersQueue::pop()
{
	playersQueue.pop();
}

Player * RingPlayersQueue::front() const
{
	return playersQueue.front();
}

size_t RingPlayersQueue::size() const
{
	return playersQueue.size();
}

bool RingPlayersQueue::haveEaqualsBets()
{
	bool result = true;
	Balance currentBet = playersQueue.front()->getBalance();
	for (int i = 0; i < playersQueue.size(); i++)
	{
		if (playersQueue.front()->getCurrentBet() != currentBet)
			result = false;
	}

	return result;
}

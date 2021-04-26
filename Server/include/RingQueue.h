#pragma once
#ifndef _RING_QUEUE_INCLUDED_
#define _RING_QUEUE_INCLUDED_

#include "Player.h"

#include <vector>
#include <algorithm>

using std::vector;

class PlayersRingQueue
{
public:
    void push(Player* newPlayer);
    Player* front() const; 
    Player* next() const;
    void pop();
    void deleteFront();

    void zeroAllBets();

    bool playerExists(const AnsiString& name) const;
    bool allBetsAreEaqual() const;
    size_t activePlayersCount() const;
	size_t nonZeroStackPlayersCount() const;
	Balance getAllBets() const;

    size_t size() const;
    const vector<Player*>& getPlayersData() const;

private:
	vector<Player*> players;
};

#endif
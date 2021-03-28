#pragma once
#ifndef _GAME_LOOP_INCLUDED_
#define _GAME_LOOP_INCLUDED_

#include <queue>

#include "Types.h"
#include "Player.h"

class PlayersQueue
{

public:

private:
};

class GameLoop
{
public:
	GameLoop(const vector<Player*>* players);
	void start();

private:
	vector<Player*>* players;
	Player* buttonPlayer;
	Player* currentPlayer;

	unsigned int bigBlind;
};

#endif
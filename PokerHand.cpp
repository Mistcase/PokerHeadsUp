#include "PokerHand.h"

PokerHand::PokerHand(Observerable* gameState, const RingPlayersQueue & players)
{
	gameState->addObserver(this);

	button = players.front();
	do
	{
		this->players.push(players.front());
		const_cast<RingPlayersQueue&>(players).next();

	} while (players.front() != button);
}

void PokerHand::play()
{
	button = players.front();

	//Blinds
	players.next();
	players.front()->makeBet(smallBlind);
	notifyObservers("SmallBlind");

	players.next();
	bb = players.front();
	players.front()->makeBet(2 * smallBlind);
	notifyObservers("BigBlind");
	players.next();


	//Торги
	//Send signal to gamestate. Ask a response for player
	
	Stage stage = STAGE_PREFLOP;
	while (players.size() > 1)
	{
		Balance maxBet = 2 * smallBlind;
		bool loopEnded = false;

		bool betsAreEaqual = players.haveEaqualsBets();
		while (!(loopEnded && betsAreEaqual))
		{
			notifyObservers("MakeDscision" + betsAreEaqual ? " CHECK BET" : " CALL RAISE FOLD");
			//WaitAnswer
			system("pause");

			if ((stage == STAGE_PREFLOP && players.front() == bb) || (stage != STAGE_PREFLOP && players.front() == button))
				loopEnded = true;
			players.next();
		}

	}
}

void PokerHand::notifyObservers(const ObsMessageString& args)
{
	ObsMessage message;
	message.sender = players.front();
	message.args = args;

	for (auto& obs : observers)
		obs->handleEvent(message);
}

void PokerHand::handleEvent(const ObsMessage & message)
{
	
}

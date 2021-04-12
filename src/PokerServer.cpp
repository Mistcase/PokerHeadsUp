#include "PokerServer.h"

PokerServer::PokerServer(const RingPlayersQueue & players)
{
	button = players.front();
	do
	{
		this->players.push(players.front());
		const_cast<RingPlayersQueue&>(players).next();

	} while (players.front() != button);
}

void PokerServer::start()
{
	std::cout << "PokerGameServer started!\n";
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


	//�����
	//Send signal to gamestate. Ask a response for player
	
	Stage stage = STAGE_PREFLOP;
	while (players.size() > 1)
	{
		currentMaxBet = 2 * smallBlind;
		bool loopEnded = false;

		bool betsAreEaqual = players.haveEaqualsBets();
		while (!(loopEnded && betsAreEaqual))
		{
			notifyObservers(Notifications::CreateNofiticationMessage("MakeDescision", (betsAreEaqual ? "CHECK_BET" : "CALL_RAISE_FOLD")));
			waitForExternalFeedback(); //External command was handled

			if ((stage == STAGE_PREFLOP && players.front() == bb) || (stage != STAGE_PREFLOP && players.front() == button))
				loopEnded = true;
			players.next();
		}
	}
}

void PokerServer::notifyObservers(const EventMessageString& args)
{
	EventMessage message;
	//message.sender = players.front();
	message.sender = this;
	message.args = args;

	for (auto& obs : observers)
		obs->handleEvent(message);
}

void PokerServer::handleEvent(const EventMessage & message)
{
	//Asnwer from GameState
	//Validate it!!!!!!
	auto action = Notifications::GetNotificationAction(message.args);
	auto args = Notifications::GetNotificationArgs(message.args);

	if (action != "ClientAnswer")
		return;

	auto currentPlayer = players.front();
	if (args == "CHECK")
	{

	}
	else if (args == "CALL")
	{
		currentPlayer->makeBet(currentMaxBet - currentPlayer->getCurrentBet());
	}
	else if (args == "BET")
	{
		Balance betValue = atoi(args.c_str());
		currentPlayer->makeBet(betValue);
		currentMaxBet = betValue;
	}
	else if (args == "RAISE")
	{
		//Args contins new max bet value
		currentMaxBet = atoi(args.c_str());
		currentPlayer->makeBet(currentMaxBet - currentPlayer->getCurrentBet());
	}
	else if (args == "FOLD")
	{
		players.pop();
	}
	else
	{
		//Error
	}

	if (args != "FOLD")
		players.next();

	//Show changes
	notifyObservers(Notifications::CreateNofiticationMessage(action, args));

	flagHaveExternalMessage = true;
}

const Player* PokerServer::getCurrentPlayer() const
{
	return players.front();
}

int PokerServer::getPlayersCount()
{
	return players.size();
}

void PokerServer::waitForExternalFeedback()
{
	while (!flagHaveExternalMessage)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	flagHaveExternalMessage = false;
}

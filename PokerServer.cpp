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


	//Торги
	//Send signal to gamestate. Ask a response for player
	
	Stage stage = STAGE_PREFLOP;
	while (players.size() > 1)
	{
		currentMaxBet = 2 * smallBlind;
		bool loopEnded = false;

		bool betsAreEaqual = players.haveEaqualsBets();
		while (!(loopEnded && betsAreEaqual))
		{
			auto currentPlayer = players.front();
			auto possiblePlayerActions = (betsAreEaqual ? "CHECK_BET" : "CALL_RAISE_FOLD");

			currentPlayer->setPossibleActions(possiblePlayerActions);
			notifyObservers(Notifications::CreateNofiticationMessage("MakeDescision", possiblePlayerActions));
			waitForExternalFeedback(); //External command was handled

			if ((stage == STAGE_PREFLOP && currentPlayer == bb) || (stage != STAGE_PREFLOP && currentPlayer == button))
				loopEnded = true;
			players.next();
		}
	}
}

void PokerServer::notifyObservers(const EventMessageString& args)
{
	for (auto& obs : observers)
		obs->handleEvent(EventMessage(this, args));
}

void PokerServer::handleEvent(const EventMessage & message)
{
	//Asnwer from GameState
	auto messageAction = Notifications::GetNotificationAction(message.args);

	if (messageAction != "ClientAnswer")
		return;

	auto playerAction = Notifications::GetNotificationArgs(message.args);
	auto currentPlayer = players.front();

	//Validating
	if (!currentPlayer->hasAction(playerAction))
	{
		notifyObservers(Notifications::CreateNofiticationMessage("ReplyingToClientAction", "Fail"));
		return;
	}

	if (playerAction == "CHECK")
	{

	}
	else if (playerAction == "CALL")
	{
		currentPlayer->makeBet(currentMaxBet - currentPlayer->getCurrentBet());
	}
	else if (playerAction == "BET")
	{
		Balance betValue = atoi(playerAction.c_str());
		currentPlayer->makeBet(betValue);
		currentMaxBet = betValue;
	}
	else if (playerAction == "RAISE")
	{
		//Args contins new max bet value
		currentMaxBet = atoi(playerAction.c_str());
		currentPlayer->makeBet(currentMaxBet - currentPlayer->getCurrentBet());
	}
	else if (playerAction == "FOLD")
	{
		players.pop();
	}
	else
	{
		//Error
	}

	//Show changes
	currentPlayer->setPossibleActions("");
	notifyObservers(Notifications::CreateNofiticationMessage("ReplyingToClientAction", "Ok"));

	flagHaveExternalMessage = true;
}

const Player* PokerServer::getCurrentPlayer() const
{
	return players.front();
}

void PokerServer::waitForExternalFeedback()
{
	while (!flagHaveExternalMessage)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	flagHaveExternalMessage = false;
}

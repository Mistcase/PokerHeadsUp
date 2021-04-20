#include "PokerLogicServer.h"

#include <iostream>

void PokerLogicServer::setPlayersCount(size_t count)
{
	//if count <= 1 || count >= 6 throw exception
	playersCount = count;
}

void PokerLogicServer::handleMessage(const AnsiString &message)
{
	if (message.empty())
		return;

	AnsiString action = Notifications::GetNotificationAction(message);

	if (action == "ConnectedPlayerName")
		handleNewConnectedPlayer(message);
	else if (action == "ClientDescision")
		handlePlayerDescision(message);

	//Update players cards info
	if (!gameStarted)
		return;

	for (auto& player : table.players.getPlayersData())
	{
		auto cards = player->getCards();
		notifyObservers(Notifications::CreateNofiticationMessage("Cards",
		{
			"PlayerId:" + to_string(player->getId()),
			"CardValue" + to_string(0) + ":" + to_string(cards.first.getValue()),
			"CardSuit" + to_string(0) + ":" + to_string(cards.first.getSuit()),
			"CardValue" + to_string(1) + ":" + to_string(cards.second.getValue()),
			"CardSuit" + to_string(1) + ":" + to_string(cards.second.getSuit()),
		}));
	}
}

void PokerLogicServer::handleNewConnectedPlayer(const AnsiString &message)
{
	constexpr Balance START_PLAYER_BALANCE = 1000;

	//Handle new connection
	AnsiString nickname = Notifications::GetNotificationArgs(message);
	AnsiString connetionStatus = "Fail";

	//If connection is valid
	PlayersRingQueue &players = table.players;
	if (!players.playerExists(nickname))
	{
		static int id = 1;
		players.push(new Player(nickname, START_PLAYER_BALANCE, id++));
		connetionStatus = "Success";
	}

	AnsiString allPlayers("");
	int counter = 0;
	for (const auto player : table.players.getPlayersData())
		allPlayers += "player" + std::to_string(counter++) + ":" + player->getNickname() + "|";

	if (table.players.size() == playersCount)
		gameStarted = true;

	notifyObservers(Notifications::CreateNofiticationMessage("PlayersInfo",
		"Status:" + connetionStatus + "|" + "PlayersCount:" + to_string(players.size()) + "|" + allPlayers));

	if (gameStarted)
	{
		stageContext.setStage(new PreflopStage(table, stageContext));
		notifyObservers(Notifications::CreateNofiticationMessage("StartGame", "-"));
		notifyObservers(Notifications::CreateNofiticationMessage("StartHand", vector<AnsiString>(
		{
			"button:" + table.button->getNickname(),
			"sbp:" + table.sb->getNickname(),
			"bbp:" + table.bb->getNickname(),
			"sbv:" + to_string(table.smallBlind),
		})));
		notifyObservers(stageContext.update(table));
	}
}

void PokerLogicServer::handlePlayerDescision(const AnsiString &message)
{
	//Handle player descision
	Player* currentPlayer = table.players.front();

	//Check if player action is possible
	//if (!currentPlayer->hasAction(playerAction))
		//return; //Ban
	AnsiString playerAction = Notifications::GetNotificationNamedArg(message, "Action");
	if (playerAction == "CHECK")
	{

	}
	else if (playerAction == "CALL")
	{
		currentPlayer->makeBet(table.currentMaxBet - currentPlayer->getCurrentBet());
	}
	else if (playerAction == "BET")
	{
		Balance betValue = atoi(playerAction.c_str());
		currentPlayer->makeBet(betValue);
		table.currentMaxBet = betValue;
	}
	else if (playerAction == "RAISE")
	{
		//Args contins new max bet value
		table.currentMaxBet = atoi(Notifications::GetNotificationNamedArg(message, "Value").c_str());
		currentPlayer->makeBet(table.currentMaxBet - currentPlayer->getCurrentBet());
	}
	else if (playerAction == "FOLD")
	{
		currentPlayer->setActive(false);
	}
	else
	{
		//Error
	}

	notifyObservers(Notifications::CreateNofiticationMessage("HideGui", "-"));
	notifyObservers(Notifications::CreateNofiticationMessage("PlayerStackInfo",
		{
			"Player:" + currentPlayer->getNickname(),
			"Balance:" + to_string(currentPlayer->getBalance()),
			"Bet:" + to_string(currentPlayer->getCurrentBet()),
		}));
	table.players.pop();

	notifyObservers(stageContext.update(table));
}

void PokerLogicServer::notifyObservers(const EventMessageString &message)
{
	for (auto &obs : observers)
		obs->handleEvent(EventMessage(this, message));
}






AnsiString PokerLogicServer::StageContext::update(TableInfo& table)
{
	PlayersRingQueue *players = &table.players;
	Player *currentPlayer = players->front();

	AnsiString retVal;
	if (!players->allBetsAreEaqual() || table.interviewedPlayers < players->size())
	{
		if (players->activePlayersCount() <= 1)
			return "GameOver";

		while (!players->front()->isActive())
			players->pop();

		retVal = stage->identifyPlayerAction();
	}
	else
	{
		setStage(new NonPreflopStage(table, *this));
		return update(table);
	}
	table.interviewedPlayers++;

	return retVal;
}

void PokerLogicServer::StageContext::setStage(Stage * newStage)
{
	stage = stage_ptr(newStage);
}




PokerLogicServer::PreflopStage::PreflopStage(TableInfo &table, StageContext& stageContext) : Stage(table, stageContext)
{
	std::cout << "PREFLOP STAGE!\n";
	
	Deck& deck = table.deck;
	deck = Deck();
	deck.shuffle();

	PlayersRingQueue &players = table.players;
	for (const auto player : players.getPlayersData())
	{
		player->setActive(true);
		player->removeAllActions();
		player->setCards(deck.removeTopCard(), deck.removeTopCard());
	}

	//handActive = true;

	//Set button player
	table.button = players.front();
	players.pop(); //Goto player after button

	Balance smallBlind = table.smallBlind;
	table.sb = players.front();
	table.sb->makeBet(smallBlind);
	//players.pop();

	//Now it is big blind
	table.bb = players.next();
	table.bb->makeBet(2 * smallBlind);

	for (int i = 0; i < 2; i++)
		players.pop();
	table.currentMaxBet = 2 * smallBlind;

	//Now players.front() is player after button(small blind)
}

AnsiString PokerLogicServer::PreflopStage::identifyPlayerAction(const AnsiString &params)
{
	PlayersRingQueue *players = &table->players;
	Player *currentPlayer = players->front();

	if (currentPlayer == table->bb)
	{

		return Notifications::CreateNofiticationMessage("MakeDescision",
			{
				"Player:" + currentPlayer->getNickname(),
				AnsiString("ButtonMode:") + (players->allBetsAreEaqual() ? "CHECK_RAISE" : "CALL_RAISE_FOLD"),
			});
	}
	else
	{
		return Notifications::CreateNofiticationMessage("MakeDescision",
		{
			"Player:" + currentPlayer->getNickname(),
			"ButtonMode:CALL_RAISE_FOLD",
		});
	}
}


PokerLogicServer::NonPreflopStage::NonPreflopStage(TableInfo &table, StageContext& stageContext) : Stage(table, stageContext)
{
	std::cout << "New nonflop STAGE!\n";
	table.currentMaxBet = 0;
	table.interviewedPlayers = 0;

	auto& players = table.players;
	players.zeroAllBets();
	while (players.front() != table.sb)
		players.pop();
}

AnsiString PokerLogicServer::NonPreflopStage::identifyPlayerAction(const AnsiString &params)
{
	PlayersRingQueue *players = &table->players;
	Player *currentPlayer = players->front();

	if (currentPlayer->getCurrentBet() < table->currentMaxBet)
	{
		return Notifications::CreateNofiticationMessage("MakeDescision",
		{
			"Player:" + currentPlayer->getNickname(),
			"ButtonMode:CALL_RAISE_FOLD",
		});
	}
	else
	{
		return Notifications::CreateNofiticationMessage("MakeDescision",
		{
			"Player:" + currentPlayer->getNickname(),
			"ButtonMode:CHECK_BET",
		});
	}
}

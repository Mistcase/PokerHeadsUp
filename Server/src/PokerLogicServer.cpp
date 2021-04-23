#include "PokerLogicServer.h"
#include <iostream>

void PokerLogicServer::setPlayersCount(size_t count)
{
	//if count <= 1 || count >= 6 throw exception
	table.expectedPlayersCount = count;
}

void PokerLogicServer::handleMessage(const AnsiString &message)
{
	if (message.empty())
		return;

	ExecutableCommand::Create(message, this)->execute();

	//Update players cards info
	if (gameStarted)
	{
		for (const auto player : table.players.getPlayersData())
			sendCommand(ptr_command(new CmdSetCards(player)));
	}
}

void PokerLogicServer::sendCommand(ptr_command command)
{
	notifyObservers(command->str());
}

void PokerLogicServer::notifyObservers(const EventMessageString &message)
{
	for (auto &obs : observers)
		obs->handleEvent(EventMessage(this, message));
}





//Stages
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


PokerLogicServer::Stage::~Stage()
{
	for (auto player : table->players.getPlayersData())
		table->pot += player->getCurrentBet();
	table->players.zeroAllBets();
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

		return Notifications::CreateNofiticationMessage("MakeDecision",
		{
			"Player:" + currentPlayer->getNickname(),
			AnsiString("ButtonMode:") + (players->allBetsAreEaqual() ? "CHECK_RAISE" : "CALL_RAISE_FOLD"),
		});
	}
	else
	{
		return Notifications::CreateNofiticationMessage("MakeDecision",
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
	while (players.front() != table.sb)
		players.pop();
}

AnsiString PokerLogicServer::NonPreflopStage::identifyPlayerAction(const AnsiString &params)
{
	PlayersRingQueue *players = &table->players;
	Player *currentPlayer = players->front();

	if (currentPlayer->getCurrentBet() < table->currentMaxBet)
	{
		return Notifications::CreateNofiticationMessage("MakeDecision",
		{
			"Player:" + currentPlayer->getNickname(),
			"ButtonMode:CALL_RAISE_FOLD",
		});
	}
	else
	{
		return Notifications::CreateNofiticationMessage("MakeDecision",
		{
			"Player:" + currentPlayer->getNickname(),
			"ButtonMode:CHECK_BET",
		});
	}
}



//Commands
PokerLogicServer::ptr_executable_command PokerLogicServer::ExecutableCommand::Create(const AnsiString & cmd, PokerLogicServer* logicServer)
{
	AnsiString action = Notifications::GetNotificationAction(cmd);

	if (action == "NewConnection")
		return ptr_executable_command(new CmdNewConnection(logicServer, cmd));
	if (action == "ClientDecision")
		return ptr_executable_command(new CmdClientDecisionRequest(logicServer, cmd));

	throw std::exception("Unknown command was received from client!");
}

AnsiString PokerLogicServer::Command::str()
{
	return cmd;
}





void PokerLogicServer::CmdNewConnection::execute()
{
	constexpr Balance START_PLAYER_BALANCE = 1000;

	//Handle new connection
	AnsiString nickname = Notifications::GetNotificationNamedArg(cmd, "Name");
	AnsiString connetionStatus = "Fail";

	//If connection is valid
	TableInfo& table = logicServer->table;
	PlayersRingQueue &players = table.players;
	if (!players.playerExists(nickname))
	{
		static int id = 1;
		players.push(new Player(nickname, START_PLAYER_BALANCE, id++));
		connetionStatus = "Success";
	}

	if (players.size() == table.expectedPlayersCount)
		logicServer->gameStarted = true;

	logicServer->sendCommand(ptr_command(new CmdPlayersInfo(logicServer)));

	if (logicServer->gameStarted)
	{
		logicServer->stageContext.setStage(new PreflopStage(table, logicServer->stageContext));
		logicServer->sendCommand(ptr_command(new CmdStartGame));
		logicServer->sendCommand(ptr_command(new CmdStartHand(logicServer)));
		logicServer->sendCommand(ptr_command(new CmdTableInfo(logicServer)));
		logicServer->notifyObservers(logicServer->stageContext.update(table));
	}
}


void PokerLogicServer::CmdClientDecisionRequest::execute()
{
	//Handle player descision
	TableInfo& table = logicServer->table;
	Player* currentPlayer = table.players.front();

	//Check if player action is possible
	//if (!currentPlayer->hasAction(playerAction))
		//return; //Ban

	AnsiString playerAction = Notifications::GetNotificationNamedArg(cmd, "Action");
	if (playerAction == "CHECK")
	{

	}
	else if (playerAction == "CALL")
	{
		currentPlayer->makeBet(table.currentMaxBet - currentPlayer->getCurrentBet());
	}
	else if (playerAction == "BET")
	{
		Balance betValue = atoi(Notifications::GetNotificationNamedArg(cmd, "Value").c_str());
		currentPlayer->makeBet(betValue);
		table.currentMaxBet = betValue;
	}
	else if (playerAction == "RAISE")
	{
		//Args contins new max bet value
		table.currentMaxBet = atoi(Notifications::GetNotificationNamedArg(cmd, "Value").c_str());
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

	logicServer->sendCommand(ptr_command(new CmdHideGui));
	table.players.pop();

	logicServer->notifyObservers(logicServer->stageContext.update(table));
	logicServer->sendCommand(ptr_command(new CmdTableInfo(logicServer))); //Send table info after stage updating
}


PokerLogicServer::CmdPlayersInfo::CmdPlayersInfo(PokerLogicServer * logicServer, const AnsiString & strCmd) : Command(logicServer, strCmd)
{
	if (logicServer == nullptr)
		return;

	auto& players = logicServer->table.players;

	AnsiString allPlayers("");
	int counter = 0;
	for (const auto player : players.getPlayersData())
		allPlayers += "player" + std::to_string(counter++) + ":" + player->getNickname() + "|";

	cmd = Notifications::CreateNofiticationMessage("PlayersInfo", "PlayersCount:" + to_string(players.size()) + "|" + allPlayers);
}


PokerLogicServer::CmdTableInfo::CmdTableInfo(PokerLogicServer * logicServer) : Command(logicServer)
{
	TableInfo& table = logicServer->table;
	AnsiString tableInfo =
		"CurrentMaxBet:" + to_string(table.currentMaxBet) + "|" +
		"Pot:" + to_string(table.pot) + "|" +
		"PlayersCount:" + to_string(table.expectedPlayersCount) + "|";

	int index = 0;
	for (const auto player : logicServer->table.players.getPlayersData())
	{
		AnsiString strIndex = to_string(index++);
		tableInfo += "Player" + strIndex + ":" + player->getNickname() + "|" +
					   "Balance" + strIndex + ":" + to_string(player->getBalance()) + "|" +
					   "Bet" + strIndex + ":" + to_string(player->getCurrentBet()) + "|";
	}

	cmd = Notifications::CreateNofiticationMessage("TableInfo", tableInfo);
}


PokerLogicServer::CmdSetCards::CmdSetCards(const Player * player) 
	: Command(nullptr, "SetCards")
{
	auto cards = player->getCards();
	cmd = Notifications::CreateNofiticationMessage("Cards",
	{
		"PlayerId:" + to_string(player->getId()),
		"CardValue" + to_string(0) + ":" + to_string(cards.first.getValue()),
		"CardSuit" + to_string(0) + ":" + to_string(cards.first.getSuit()),
		"CardValue" + to_string(1) + ":" + to_string(cards.second.getValue()),
		"CardSuit" + to_string(1) + ":" + to_string(cards.second.getSuit()),
	});
}


PokerLogicServer::CmdStartHand::CmdStartHand(PokerLogicServer * logicServer) 
	: Command(logicServer)
{
	TableInfo& table = logicServer->table;
	cmd = Notifications::CreateNofiticationMessage("StartHand", 
	{
		"button:" + table.button->getNickname(),
		"sbp:" + table.sb->getNickname(),
		"bbp:" + table.bb->getNickname(),
		"sbv:" + to_string(table.smallBlind),
	});
}

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
void PokerLogicServer::StageContext::update()
{
	TableInfo& table = logicServer->table;
	PlayersRingQueue *players = &table.players;
	Player *currentPlayer = players->front();

	if (!players->allBetsAreEaqual() || table.interviewedPlayers < players->size())
	{
		while (!players->front()->isActive())
			players->pop();

		if (players->activePlayersCount() <= 1)
		{
			logicServer->sendCommand(ptr_command(new CmdSetWinners({players->front()}, table.pot)));
			setStage(new VictoryStage(logicServer));
			stage->update();
			return;
		}

		stage->update();
	}
	else
	{
		//////////////////////////////////////////////////////////
		int stageId = stage->getId();

		if (stageId == StageId::STAGE_RIVER)
			setStage(new VictoryStage(logicServer));
		else
			setStage(new NonPreflopStage(logicServer, static_cast<StageId>(stageId + 1)));

		update();
		return;
	}
	table.interviewedPlayers++;
}

void PokerLogicServer::StageContext::setStage(Stage * newStage)
{
	stage = stage_ptr(newStage);
}


PokerLogicServer::Stage::Stage(PokerLogicServer * logicServer)
	: logicServer(logicServer)
{
	TableInfo& table = logicServer->table;
	for (auto player : table.players.getPlayersData())
		table.pot += player->getCurrentBet();

	table.players.zeroAllBets();

	table.currentMaxBet = 0;
	table.interviewedPlayers = 0;
}

PokerLogicServer::StageId PokerLogicServer::Stage::getId()
{
	return id;
}




PokerLogicServer::PreflopStage::PreflopStage(PokerLogicServer* logicServer)
	: Stage(logicServer)
{
	std::cout << "PREFLOP STAGE!\n";

	id = StageId::STAGE_PREFLOP;

	TableInfo& table = logicServer->table;
	Deck& deck = table.deck;
	deck = Deck();
	deck.shuffle();

	PlayersRingQueue &players = table.players;
	for (const auto player : players.getPlayersData())
	{
		player->setActive(true);
		player->removeAllActions();
		player->setCards(deck.removeTopCard(), deck.removeTopCard());
		logicServer->sendCommand(ptr_command(new CmdSetCards(player)));
	}

	//Set button player
	table.button = players.front();
	players.pop(); //Goto player after button

	Balance smallBlind = table.smallBlind;
	table.sb = players.front();
	table.sb->makeBet(smallBlind);
	table.pot = 0;
	
	//Now it is big blind
	table.bb = players.next();
	table.bb->makeBet(2 * smallBlind);

	for (int i = 0; i < 2; i++)
		players.pop();
	table.currentMaxBet = 2 * smallBlind;

	logicServer->sendCommand(ptr_command(new CmdStartHand(logicServer)));
}

void PokerLogicServer::PreflopStage::update()
{
	TableInfo& table = logicServer->table;
	PlayersRingQueue *players = &table.players;
	Player *currentPlayer = players->front();

	if (currentPlayer == table.bb)
		logicServer->sendCommand(ptr_command(new CmdClientDecisionRequest(currentPlayer, (players->allBetsAreEaqual() ? "CHECK_RAISE" : "CALL_RAISE_FOLD"))));
	else
		logicServer->sendCommand(ptr_command(new CmdClientDecisionRequest(currentPlayer, "CALL_RAISE_FOLD")));
}


PokerLogicServer::NonPreflopStage::NonPreflopStage(PokerLogicServer* logicServer, StageId id) : Stage(logicServer)
{
	std::cout << "New nonflop STAGE!\n";
	this->id = id;

	logicServer->sendCommand(ptr_command(new CmdOpenBoardCards(logicServer->table, id)));

	TableInfo& table = logicServer->table;
	table.currentMaxBet = 0;
	table.interviewedPlayers = 0;

	auto& players = table.players;
	while (players.front() != table.sb)
		players.pop();
}

void PokerLogicServer::NonPreflopStage::update()
{
	TableInfo& table = logicServer->table;
	PlayersRingQueue *players = &table.players;
	Player *currentPlayer = players->front();

	logicServer->sendCommand(ptr_command(new CmdClientDecisionRequest(currentPlayer, (currentPlayer->getCurrentBet() < table.currentMaxBet) ? "CALL_RAISE_FOLD" : "CHECK_BET")));
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
		logicServer->sendCommand(ptr_command(new CmdStartGame));
		logicServer->stageContext.setStage(new PreflopStage(logicServer));
		logicServer->sendCommand(ptr_command(new CmdTableInfo(logicServer)));
		logicServer->stageContext.update();
		//logicServer->notifyObservers(logicServer->stageContext.update());
	}
}

PokerLogicServer::CmdClientDecisionRequest::CmdClientDecisionRequest(const Player* player, const AnsiString& buttonsNotation)
	: ExecutableCommand(nullptr, "")
{
	cmd = Notifications::CreateNofiticationMessage("MakeDecision", 
	{
		"Player:" + player->getNickname(),
		"ButtonMode:" + buttonsNotation,
	});
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

	logicServer->stageContext.update();
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


PokerLogicServer::CmdOpenBoardCards::CmdOpenBoardCards(TableInfo & table, StageId stageId)
{
	AnsiString stage;
	vector<size_t> cardsIndexes;
	switch (stageId)
	{
	case PokerLogicServer::STAGE_FLOP:
		stage = "Flop";
		cardsIndexes = { 0, 1, 2 };
		break;

	case PokerLogicServer::STAGE_TURN:
		stage = "Turn";
		cardsIndexes = { 3 };
		break;

	case PokerLogicServer::STAGE_RIVER:
		stage = "River";
		cardsIndexes = { 4 };
		break;

	default:
		stage = "STAGE_UNKNOWN(ERROR)";
		break;
	}

	AnsiString cardsStringArgs;
	for (size_t index : cardsIndexes)
	{
		table.boardCards[index] = table.deck.removeTopCard();
		cardsStringArgs += "CardValue" + to_string(index) + ":" + to_string(table.boardCards[index].getValue()) + "|";
		cardsStringArgs += "CardSuit" + to_string(index) + ":" + to_string(table.boardCards[index].getSuit()) + "|";
	}

	cmd = Notifications::CreateNofiticationMessage("OpenBoardCards", "Stage:" + stage + "|" + cardsStringArgs);
}


PokerLogicServer::CmdSetWinners::CmdSetWinners(const vector<const Player*>& players, Balance pot)
{
	AnsiString allPlayers;

	for (size_t i = 0; i < players.size(); i++)
		allPlayers += "Player" + to_string(i) + ":" + players[i]->getNickname() + "|";

	cmd = Notifications::CreateNofiticationMessage("SetWinners",
	{
		"Count:" + to_string(players.size()),
		"Gain:" + to_string(pot / players.size()),
		allPlayers
	});
}


PokerLogicServer::VictoryStage::VictoryStage(PokerLogicServer * logicServer)
	: Stage(logicServer)
{
	id = STAGE_WINNER_EXISTS;
}

void PokerLogicServer::VictoryStage::update()
{
	TableInfo& table = logicServer->table;
	auto& players = table.players;

	if (players.activePlayersCount() > 1)
	{
		//Identify Winner
		identifyWinners();
		for (auto winner : winners)
			winner->setBalance(winner->getBalance() + table.pot / winners.size());
	}

	if (players.activePlayersCount() == 1)
	{
		Player* frontPlayer = players.front();
		frontPlayer->setBalance(frontPlayer->getBalance() + table.pot);
	}

	players.pop();

	auto& stageContext = logicServer->stageContext;
	stageContext.setStage(new PreflopStage(logicServer));
	stageContext.update();
}

void PokerLogicServer::VictoryStage::identifyWinners()
{
	TableInfo& table = logicServer->table;

	typedef pair<Player*, CombinationIdentifier::Combination> PlayerCombination;
	vector<PlayerCombination> pCombinations;
	for (const auto player : table.players.getPlayersData())
		pCombinations.emplace_back(PlayerCombination(player, CombinationIdentifier(player->getCards(), table.boardCards).identify()));

	for (const auto& playerComb : pCombinations)
	{
		bool flag = true;
		for (const auto& subComb : pCombinations)
		{
			if (&playerComb == &subComb)
				continue;

			if (playerComb.second < subComb.second)
			{
				flag = false;
				break;
			}
		}
		if (flag)
			winners.emplace_back(playerComb.first);
	}
}

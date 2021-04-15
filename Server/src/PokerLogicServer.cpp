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
        handlePlayerDescision(Notifications::GetNotificationArgs(message));
}

void PokerLogicServer::handleNewConnectedPlayer(const AnsiString &message)
{
    constexpr Balance START_PLAYER_BALANCE = 1000;

    //Handle new connection
    AnsiString nickname = Notifications::GetNotificationArgs(message);
    AnsiString connetionStatus = "Fail";

    //If connection is valid
    auto players = table.players;
    if (!players.playerExists(nickname))
    {
        players.push(new Player(nickname, START_PLAYER_BALANCE, 1)); //Not 1
        connetionStatus = "Success";
    }

    AnsiString allPlayers("");
    int counter = 0;
    for (const auto player : table.players.getPlayersData())
        allPlayers += "player" + std::to_string(counter++) + ":" + player->getNickname() + "|";

    if (table.players.size() == playersCount)
        gameStarted = true;

    notifyObservers(Notifications::CreateNofiticationMessage("PlayersInfo",
        "Status:" + connetionStatus + "|" + "PlayersCount:" + std::to_string(players.size()) + "|" + allPlayers));

    if (gameStarted)
    {
        notifyObservers(Notifications::CreateNofiticationMessage("StartGame", "-"));
        startNewHand();
        identifyNextHandAction();
    }
}

void PokerLogicServer::handlePlayerDescision(const AnsiString &playerAction)
{

    // //Handle player descision
    // Player* currentPlayer = players.front();

    // //Check if player action is possible
    // if (!currentPlayer->hasAction(playerAction))
    //     return; //Ban

    // if (playerAction == "CHECK")
	// {
        
	// }
	// else if (playerAction == "CALL")
	// {
	// 	currentPlayer->makeBet(currentMaxBet - currentPlayer->getCurrentBet());
	// }
	// else if (playerAction == "BET")
	// {
	// 	Balance betValue = atoi(playerAction.c_str());
	// 	currentPlayer->makeBet(betValue);
	// 	currentMaxBet = betValue;
	// }
	// else if (playerAction == "RAISE")
	// {
	// 	//Args contins new max bet value
	// 	currentMaxBet = atoi(playerAction.c_str());
	// 	currentPlayer->makeBet(currentMaxBet - currentPlayer->getCurrentBet());
	// }
	// else if (playerAction == "FOLD")
	// {
	// 	currentPlayer->setActive(false);
	// }
	// else
	// {
	// 	//Error
	// }

    // notifyObservers(Notifications::CreateNofiticationMessage("HideGui", "-"));
    // notifyObservers(Notifications::CreateNofiticationMessage("PlayerStackInfo", 
    // {
    //     "Player:" + currentPlayer->getNickname(),
    //     "Balance:" + to_string(currentPlayer->getBalance()), 
    //     "Bet:" + to_string(currentPlayer->getCurrentBet()),
    // }));
    // players.pop();
    // identifyNextHandAction();
}

// AnsiString PokerLogicServer::createPlayersInfo()
// {
//     AnsiString args("");
//     size_t counter = 0;
//     for (const auto player : players.getPlayersData())
//         args += "player" + std::to_string(counter++) + ":" + player->getNickname() + "|";
//     return Notifications::CreateNofiticationMessage("PlayersInfo", args);
// }

void PokerLogicServer::notifyObservers(const EventMessageString &message)
{
    for (auto &obs : observers)
        obs->handleEvent(EventMessage(this, message));
}

void PokerLogicServer::startNewHand()
{
    // for (const auto player : players.getPlayersData())
    // {
    //     player->setActive(true);
    //     player->removeAllActions();
    // }

    // handActive = true;
    // handStage = STAGE_PREFLOP;

    // //Set button player
    // button = players.front();
    // players.pop(); //Goto player after button

    // sb = players.front();
    // sb->makeBet(smallBlind);
    // //players.pop();

    // //Now it is big blind
    // bb = players.next();
    // bb->makeBet(2 * smallBlind);

    // for (int i = 0; i < 2; i++)
    //     players.pop();
    // loopStartPlayer = players.front();
    // currentMaxBet = 2 * smallBlind;

    // //Now players.front() is player after button(small blind)
    // notifyObservers(Notifications::CreateNofiticationMessage("StartHand", vector<AnsiString>(
    // {
    //     "button:" + button->getNickname(),
    //     "sbp:" + sb->getNickname(),
    //     "bbp:" + bb->getNickname(),
    //     "sbv:" + to_string(smallBlind),
    // }
    // )));

    // if (players.size() == 2)
    //     onceComplited = true;
}

bool PokerLogicServer::identifyNextHandAction()
{
    
    return true;
}

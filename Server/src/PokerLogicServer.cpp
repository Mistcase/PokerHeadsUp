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
        stageContext = StageContext(new PreflopStage(table, stageContext));
        notifyObservers(Notifications::CreateNofiticationMessage("StartGame", "-"));
        notifyObservers(Notifications::CreateNofiticationMessage("StartHand", vector<AnsiString>(
                                                                                  {
                                                                                      "button:" + table.button->getNickname(),
                                                                                      "sbp:" + table.sb->getNickname(),
                                                                                      "bbp:" + table.bb->getNickname(),
                                                                                      "sbv:" + to_string(table.smallBlind),
                                                                                  })));
        notifyObservers(stageContext->makeLoopAction());
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

    notifyObservers(stageContext->makeLoopAction());
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




PokerLogicServer::PreflopStage::PreflopStage(TableInfo &table, StageContext& stageContext) : Stage(table, stageContext)
{
     std::cout << "PREFLOP STAGE!\n";

    PlayersRingQueue &players = table.players;
    for (const auto player : players.getPlayersData())
    {
        player->setActive(true);
        player->removeAllActions();
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

AnsiString PokerLogicServer::PreflopStage::makeLoopAction(const AnsiString &params)
{
    PlayersRingQueue *players = &table->players;
    Player *currentPlayer = players->front();

    AnsiString retVal;

    if (!players->allBetsAreEaqual() || table->interviewedPlayers < players->size())
    {
        if (players->activePlayersCount() <= 1)
            return "GameOver";

        while (!players->front()->isActive())
            players->pop();

        if (currentPlayer == table->bb)
        {
            // if (players->allBetsAreEaqual())
            // {

            // }
            // currentPlayer->addAction("CHECK");
            // currentPlayer->addAction("RAISE");
            // currentPlayer->addAction("FOLD");

            retVal = Notifications::CreateNofiticationMessage("MakeDescision",
            {
                "Player:" + currentPlayer->getNickname(),
                AnsiString("ButtonMode:") + (players->allBetsAreEaqual() ? "CHECK_RAISE" : "CALL_RAISE_FOLD"),
            });
        }
        else 
        {
            retVal = Notifications::CreateNofiticationMessage("MakeDescision",
            {
                "Player:" + currentPlayer->getNickname(),
                "ButtonMode:CALL_RAISE_FOLD",
            });
        }
    }
    else
    {
        *stageContext = StageContext(new FlopStage(*table, *stageContext));
        return (*stageContext)->makeLoopAction();
    }
    table->interviewedPlayers++;
    return retVal;
}


PokerLogicServer::FlopStage::FlopStage(TableInfo &table, StageContext& stageContext) : Stage(table, stageContext)
{
    std::cout << "FLOP STAGE!\n";
    table.currentMaxBet = 0;
    table.interviewedPlayers = 0;

    auto& players = table.players;
    players.zeroAllBets();
    while (players.front() != table.sb)
        players.pop(); 
}

AnsiString PokerLogicServer::FlopStage::makeLoopAction(const AnsiString &params)
{
    PlayersRingQueue *players = &table->players;
    Player *currentPlayer = players->front();

    AnsiString retVal;
    if (!players->allBetsAreEaqual() || table->interviewedPlayers < players->size())
    {
        if (players->activePlayersCount() <= 1)
            return "GameOver";

        while (!players->front()->isActive())
            players->pop();

        if (currentPlayer->getCurrentBet() < table->currentMaxBet)
        {
            retVal = Notifications::CreateNofiticationMessage("MakeDescision",
            {
                "Player:" + currentPlayer->getNickname(),
                "ButtonMode:CALL_RAISE_FOLD",
            });
        }
        else
        {
            retVal = Notifications::CreateNofiticationMessage("MakeDescision",
            {
                "Player:" + currentPlayer->getNickname(),
                "ButtonMode:CHECK_BET",
            });
        }
    }
    else
    {
        //Create new stage
    }
    table->interviewedPlayers++;
    
    return retVal;
}
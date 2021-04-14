#include "PokerLogicServer.h"

void PokerLogicServer::handleMessage(const string& message)
{
    if (Notifications::GetNotificationAction(message) == "ConnectedPlayerName")
        handleNewConnectedPlayer(message);
}

void PokerLogicServer::handleNewConnectedPlayer(const string& message)
{
    //Handle new connection
}

void PokerLogicServer::handlePlayerDescision(const string& message)
{
    //Handle player descision
}
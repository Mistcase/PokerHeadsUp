#ifndef _POKER_LOGIC_SERVER_INCLUDED_
#define _POKER_LOGIC_SERVER_INCLUDED_

#include "Notifications.h"

#include <string>

using std::string;

class PokerLogicServer
{
public:
    void handleMessage(const string& message);
    
private:
    void handleNewConnectedPlayer(const stirng& message);
    void handlePlayerDescision(const string& message);
};

#endif
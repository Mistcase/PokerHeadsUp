#ifndef _COMMON_SERVER_INCLUDED_
#define _COMMON_SERVER_INCLUDED_

#include "PokerLogicServer.h"

#include <TcpServer.hpp>

#include <string>
#include <vector>

using std::vector;
using std::string;
using netboost::Address;
using netboost::TcpServer;
using netboost::ConnectionDescriptor;
using netboost::ConnectionOpeningException;

class Server
{
public:
    Server(Server&) = delete;
    Server(Server&&) = delete;

    Server(const string& fullAddr, size_t playersCount);
    void start();

private:
    PokerLogicServer pokerLogicServer;

    TcpServer* tcpServer;
    vector<ConnectionDescriptor> connections;

    bool active = true;
};

#endif
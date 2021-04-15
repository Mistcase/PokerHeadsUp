#ifndef _COMMON_SERVER_INCLUDED_
#define _COMMON_SERVER_INCLUDED_

#include "PokerLogicServer.h"

#include <TcpServer.hpp>

#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;
using netboost::Packet;
using netboost::Address;
using netboost::TcpServer;
using netboost::ConnectionDescriptor;
using netboost::ConnectionOpeningException;

class Server : public Observer
{
public:
    Server(Server&) = delete;
    Server(Server&&) = delete;

    Server(const string& fullAddr, size_t playersCount);
    void start();

    void handleEvent(const EventMessage& message) override;
    
private:
    void sendMessage(ConnectionDescriptor connection, const AnsiString& message);
    void sendMessageToAll(const AnsiString& message);

private:
    PokerLogicServer pokerLogicServer;

    Address endPointAddr;
    TcpServer* tcpServer;
    vector<ConnectionDescriptor> connections;

    bool active = true;
};

#endif
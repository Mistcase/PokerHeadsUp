#ifndef _NET_CLIENT_INCLUDED_
#define _NET_CLIENT_INCLUDED_

#include "Types.h"

#include <TcpClient.hpp>

class NetClient
{
public:
    NetClient();
    ~NetClient();

    bool connect(const netboost::Address& serverAddr);
    bool sendMessage(const AnsiString& message);
    bool hasMessage() const;
    AnsiString receiveMessage();

private:
    netboost::TcpClient* tcpClient;
    netboost::ConnectionDescriptor connectionDescriptor;
    netboost::Address currentServerAddr;
};

#endif
#include "NetClient.h"

NetClient::NetClient()
{
    tcpClient = new netboost::TcpClient;
}

NetClient::~NetClient()
{
    delete tcpClient;
}

bool NetClient::connect(const netboost::Address& serverAddr)
{
    currentServerAddr = serverAddr;
    connectionDescriptor = tcpClient->connect(serverAddr);

    return connectionDescriptor != netboost::n_codes::ANY_ERROR;
}

bool NetClient::sendMessage(const AnsiString& message)
{
    return tcpClient->write(connectionDescriptor, netboost::Packet(message.c_str(), message.length()));
}

bool NetClient::hasMessage() const
{
    return tcpClient->readable(connectionDescriptor);
}

bool NetClient::active() const
{
    return tcpClient->active(connectionDescriptor);
}

AnsiString NetClient::receiveMessage()
{
    return AnsiString(static_cast<const char*>(tcpClient->readPacket(connectionDescriptor).getData()));
}

void NetClient::disconnect()
{
    tcpClient->disconnect(connectionDescriptor);
}
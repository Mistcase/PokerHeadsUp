#include "Server.h"

Server::Server(const string &fullAddr, size_t playersCount)
{
    try
    {
        tcpServer = new TcpServer(Address(fullAddr));

        for (int i = 0; i < playersCount; i++)
            connections.emplace_back(tcpServer->openConnection());
    }
    catch (const ConnectionOpeningException &e)
    {
        std::cerr << e.what() << '\n';
        exit(-1);
    }
}

void Server::start()
{
    for (auto conn : connections)
        tcpServer->accept(conn);

    //StartgameLogic server
    while (active)
    {
        for (auto conn : connections)
        {
            if (tcpServer->readable(conn))
                pokerLogicServer.handleMessage(static_cast<const char*>(tcpServer->readPacket(conn).getData()));
        }
    }
}
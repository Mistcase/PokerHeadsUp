#include "Server.h"

Server::Server(const string &fullAddr, size_t playersCount)
{
    try
    {
        endPointAddr = fullAddr;
        tcpServer = new TcpServer(Address(fullAddr));

        for (int i = 0; i < playersCount; i++)
            connections.emplace_back(tcpServer->openConnection());
    }
    catch (const ConnectionOpeningException &e)
    {
        std::cerr << e.what() << std::endl;
        exit(-1);
    }
}

void Server::start()
{
    std::cout << "Starting Server: " << endPointAddr.getAddress(true) << std::endl;
    for (const auto conn : connections)
        tcpServer->accept(conn);

    //StartgameLogic server
    while (active)
    {
        for (const auto conn : connections)
        {
            if (tcpServer->readable(conn))
                std::cout << std::string(static_cast<const char*>(tcpServer->readPacket(conn).getData())) << std::endl;
                //pokerLogicServer.handleMessage(static_cast<const char*>(tcpServer->readPacket(conn).getData()));
        }
    }
}
#include "Server.h"

Server::Server(const string &fullAddr, size_t playersCount)
{
    try
    {
        pokerLogicServer.setPlayersCount(playersCount);

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

    std::thread([&]()
    {
        for (const auto conn : connections)
            tcpServer->accept(conn);
    }).detach();
    
    //StartgameLogic server
    pokerLogicServer.addObserver(this);
    while (active)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        for (const auto conn : connections)
        {
            if (tcpServer->readable(conn))
            {
                pokerLogicServer.handleMessage(static_cast<const char*>(tcpServer->readPacket(conn).getData()));
            }
        }
    }
}

void Server::handleEvent(const EventMessage& message)
{
    AnsiString action = Notifications::GetNotificationAction(message.params);
    if (action == "AddNewPlayer")
    {
        if (Notifications::GetNotificationNamedArg(message.params, "Status") == "Fail")
        {
            //Handle failed connection
            std::cout << "Connection Failed!\n";
            return;
        }
    }
    sendMessageToAll(message.params);
    
}

void Server::sendMessage(ConnectionDescriptor connection, const AnsiString& message)
{
    std::cout << "Message to connection(" << connection << "): " << message << std::endl;
    tcpServer->write(connection, Packet(message.c_str(), message.length() + 1));
}

void Server::sendMessageToAll(const AnsiString& message)
{
    std::cout << "Message to everyone: " << message << std::endl;
    std::cout << "-----------------------------------------------\n";
    for (const auto conn : connections)
    {
        if (tcpServer->active(conn))
            sendMessage(conn, message);
    }
    std::cout << "-----------------------------------------------\n\n";
}
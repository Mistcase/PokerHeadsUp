#include "TcpServer.hpp"

netboost::TcpServer::TcpServer(const Address& address)
{
    if (listenSocket.bind(address.getAddress(false).c_str(), address.getPort()) == n_codes::SOCKET_BIND_ERROR)
    {
        throw ConnectionOpeningException("BIND ERROR: " + address.getAddress(true));
    }
    if (listenSocket.listen(SOMAXCONN) != n_codes::SUCCESS)
    {
        throw ConnectionOpeningException("LISTEN ERROR: " + address.getAddress(true));
    }
}

netboost::ConnectionDescriptor netboost::TcpServer::openConnection()
{
    for (int i = 0; i < 256; i++)
    {
        if (!(sockMask & (1ULL << i)))
        {
            sockets.at(i) = new TcpSocket();
            sockMask |= (1ULL << i);
            return i + 1;
        }
    }
    return n_codes::ANY_ERROR;
}

bool netboost::TcpServer::accept(netboost::ConnectionDescriptor descriptor)
{
    int index = descriptor - 1;
    if ((index < 0) || (!(sockMask & (1ULL << index))))
    {
        return false;
    }
    *sockets.at(index) = listenSocket.accept();
    return true;
}
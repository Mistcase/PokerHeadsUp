#include "TcpClient.hpp"

netboost::ConnectionDescriptor netboost::TcpClient::connect(const Address &address)
{
    for (int i = 0; i < 256; i++)
    {
        if (!(sockMask & (1ULL << i)))
        {
            sockets.at(i) = new TcpSocket();
            if (sockets.at(i)->connect(address) != n_codes::SUCCESS)
                return 0;
            sockMask |= (1ULL << i);
            return i + 1;
        }
    }
    return 0;
}
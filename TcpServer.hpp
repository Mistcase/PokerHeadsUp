#ifndef TcpServer_hpp
#define TcpServer_hpp

#include "TcpEntity.hpp"
#include "Address.hpp"

namespace netboost
{

    class TcpServer : public TcpEntity
    {
    public:
        TcpServer() = default;
        TcpServer(const Address &address);

        netboost::ConnectionDescriptor openConnection();

        bool accept(ConnectionDescriptor descriptor);

    private:
        TcpSocket listenSocket;
    };

}

#endif /* TcpServer_hpp */

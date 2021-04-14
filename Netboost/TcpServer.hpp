#ifndef TcpServer_hpp
#define TcpServer_hpp

#include "TcpEntity.hpp"
#include "Address.hpp"

namespace netboost
{
    class ConnectionOpeningException
    {
    private:
        std::string m_error;

    public:
        ConnectionOpeningException(std::string error)
            : m_error(error)
        {
        }

        const std::string& what() const noexcept
        {
            return m_error;
        }
    };


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

#endif
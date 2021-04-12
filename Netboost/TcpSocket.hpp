#ifndef TcpSocket_hpp
#define TcpSocket_hpp

#include <cstdint>
#include <ctime>
#include <iostream>

#include "Socket.hpp"

namespace netboost
{
    class TcpSocket : public BaseSocket{
    public:
        int bind(const char *ipAdress, int port) noexcept;

        int listen(int s) noexcept;

        TcpSocket accept() noexcept;

        int connect(const char *ipAdress, int port) noexcept;

        int connect(const Address &address) noexcept;

        void disconnect() const noexcept;

        bool connected() const noexcept;

        bool readable() const noexcept;

        int send(void *data, size_t dataSize) noexcept;

        int receive(void *data, int dataSize) noexcept;

        int nativeHandle() const noexcept;
    };
}
#endif

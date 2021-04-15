#ifndef NETBOOST_TCPENTITY_HPP
#define NETBOOST_TCPENTITY_HPP

#include <array>
#include <bitset>

#include "netinit.hpp"
#include "TcpSocket.hpp"
#include "Packet.hpp"

namespace netboost
{
    class TcpEntity{
    public:
        bool active(ConnectionDescriptor descriptor) const;

        bool readable(ConnectionDescriptor descriptor) const;

        template <class T>
        size_t write(ConnectionDescriptor descriptor, const T& data, int dataSize = sizeof(T));
        bool write(ConnectionDescriptor descriptor, const Packet& packet);

        Packet readPacket(ConnectionDescriptor descriptor);
        int read(ConnectionDescriptor descriptor, void *buffer, int bufferSize);

        void disconnect(ConnectionDescriptor descriptor);

    protected:
        TcpEntity() = default;

        uint64_t sockMask = 0;
        std::array<TcpSocket*, 256> sockets;
    };





    template<class T>
    inline size_t TcpEntity::write(ConnectionDescriptor descriptor, const T& data, int dataSize)
    {
        int index = descriptor - 1;
        if ((index < 0) || !(sockMask & (1ULL << index)))
        {
            return 0;
        }
        if (sockets.at(index)->send(&dataSize, sizeof(int)) == sizeof(int))
        {
            return sockets.at(index)->send((void*)(&data), dataSize);
        }
        std::cout << "DROP!\n";
        return 0;
    }
}

#endif

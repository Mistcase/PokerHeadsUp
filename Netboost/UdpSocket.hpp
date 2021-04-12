#ifndef UdpSocket_hpp
#define UdpSocket_hpp

#include "Socket.hpp"
#include "netinit.hpp"

namespace netboost
{
    class UdpSocket : public BaseSocket
    {
    public:
        UdpSocket();

        bool open(const Address & address);
        bool setNonBlockMode(bool flag = true);
        void close();
        bool isOpen() const;
        bool send(const Address & destination, const void *data, int size );
        int receive(Address* sender, void * data, int size );
    };
}

#endif /* UdpSocket_hpp */

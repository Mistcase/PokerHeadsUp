#include "UdpSocket.hpp"

netboost::UdpSocket::UdpSocket()
{
    handle = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

bool netboost::UdpSocket::open(const Address & bindAddr)
{
    address = bindAddr;

    if ( handle <= 0 )
    {
        return false;
    }

    if (bind(handle, (const sockaddr*)&address, sizeof(sockaddr_in) ) < 0 )
    {
        return false;
    }
    
    return true;
}

bool netboost::UdpSocket::setNonBlockMode(bool nonblock)
{
        if (nonblock)
        {
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

            int nonBlocking = 1;
            if (fcntl(handle, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
            {
                return false;
            }

#elif PLATFORM == PLATFORM_WINDOWS
            DWORD nonBlocking = 1;
        if ( ioctlsocket( handle, FIONBIO, &nonBlocking ) != 0 )
        {
            printf( "failed to set non-blocking socket\n" );
            return false;
        }

#endif
        }
    else
    {
        //Set blocking mode!
    }
}

void netboost::UdpSocket::close()
{
    #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
        ::close(handle);
    #elif PLATFORM == PLATFORM_WINDOWS
        ::closesocket(handle);
    #endif
}

bool netboost::UdpSocket::isOpen() const
{
    return true;
}

bool netboost::UdpSocket::send(const Address & destination, const void * data, int size)
{
    return ::sendto(handle, (const char*)data, size, 0, (sockaddr*)address.getSockAddr(), sizeof(sockaddr_in)) == size;
}

int netboost::UdpSocket::receive(Address* sender, void * data, int size )
{
#if PLATFORM == PLATFORM_WINDOWS
    typedef int socklen_t;
#endif
    
    sockaddr_in from;
    socklen_t fromLength = sizeof(from);
    
    long received_bytes = recvfrom(handle, (char*)data, size, 0, (sockaddr*)&from, &fromLength);
    
    if ( received_bytes <= 0 )
        return -1;
    
    unsigned int from_address = ntohl( from.sin_addr.s_addr );
    unsigned int from_port = ntohs( from.sin_port );
    *sender = Address(from_address, from_port);
    
    return static_cast<int>(received_bytes);
}

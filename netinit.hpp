#pragma once
#ifndef _CAT_NETWORK_INCLUDED_
#define _CAT_NETWORK_INCLUDED_

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <cstdint>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <poll.h>

#endif

#include "ncodes.hpp"

typedef unsigned char byte;
typedef int Socket;

namespace netboost
{
    typedef int ConnectionDescriptor;
    constexpr size_t MAX_ACTIVE_CONNECTIONS = 32;

    inline bool Initialize()
    {
#if PLATFORM == PLATFORM_WINDOWS
        WSADATA WsaData;
        return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
#else
        return true;
#endif
    }

    inline void ReleaseResources()
    {
#if PLATFORM == PLATFORM_WINDOWS
        WSACleanup();
#endif
    }
    inline int poll(pollfd* pfd, int nfds, int timeout)
    {
#if PLATFORM == PLATFORM_WINDOWS
        return WSAPoll(pfd, nfds, timeout);
#else
        return ::poll(pfd, nfds, timeout);
#endif
    }
}

#endif

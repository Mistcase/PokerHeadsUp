#ifndef NETBOOST_SOCKET_H
#define NETBOOST_SOCKET_H

#include "netinit.hpp"

#include "Address.hpp"

namespace netboost
{
    class BaseSocket {
    public:

    protected:
        BaseSocket() = default;

        Socket handle;
        Address address;
    };
}

#endif

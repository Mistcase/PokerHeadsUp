#ifndef TcpCatCodes_h
#define TcpCatCodes_h

typedef int netboost_code;

namespace netboost
{
    namespace n_codes
    {
        constexpr int SUCCESS = 1;
        constexpr int ANY_ERROR = 0;
        constexpr int SOCKET_BIND_ERROR = -1;
        constexpr int SOCKET_ACCEPTION_ERROR = -2;
        constexpr int SOCKET_CREATION_ERROR = -3;
        constexpr int CONNECTION_FAILED = -4;
    }
}

#endif

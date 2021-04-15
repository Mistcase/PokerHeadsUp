#include "TcpSocket.hpp"

int netboost::TcpSocket::bind(const char* ipAddress, int port) noexcept
{
    address = Address(ipAddress, port);
    handle = socket(AF_INET, SOCK_STREAM, 0);

    return (::bind(handle, (sockaddr*)address.getSockAddr(), sizeof(sockaddr_in)) < 0) ? netboost::n_codes::SOCKET_BIND_ERROR : netboost::n_codes::SUCCESS;
}

int netboost::TcpSocket::listen(int s) noexcept
{
    if (handle < 0)
        return netboost::n_codes::SOCKET_CREATION_ERROR;
    ::listen(handle, s);
    
    return netboost::n_codes::SUCCESS;
}

netboost::TcpSocket netboost::TcpSocket::accept() noexcept
{
    TcpSocket newhandle;
    socklen_t addrSize = sizeof(sockaddr_in);
    newhandle.handle = ::accept(handle, (sockaddr*)address.getSockAddr(), &addrSize);
    newhandle.m_connected = true;
    return newhandle;
}

int netboost::TcpSocket::connect(const char* ipAddress, int port) noexcept
{
    handle = socket(AF_INET, SOCK_STREAM, 0);
    if (handle < 0)
        return netboost::n_codes::SOCKET_CREATION_ERROR;

    address = Address(ipAddress, port);

    int cresult = (::connect(handle, (sockaddr*)address.getSockAddr(), sizeof(sockaddr_in)) < 0) ? netboost::n_codes::CONNECTION_FAILED : netboost::n_codes::SUCCESS;
    m_connected = (cresult == n_codes::SUCCESS);

    return cresult;
}

int netboost::TcpSocket::connect(const Address& address) noexcept
{
    return connect((std::to_string(address.getA()) + "." + std::to_string(address.getB()) + "." + std::to_string(address.getC()) + "." + std::to_string(address.getD())).c_str(), address.getPort());
}

void netboost::TcpSocket::disconnect() const noexcept
{
    shutdown(handle, 2);
    
#if PLATFORM == PLATFORM_WINDOWS
    ::closesocket(handle);
#else
    ::close(handle);
#endif
}

bool netboost::TcpSocket::connected() const noexcept
{
    return m_connected;
}

bool netboost::TcpSocket::readable() const noexcept
{
    if (!connected())
        return false;

    pollfd fds;
    fds.fd = handle;
    fds.events = POLLIN;
    
    int ret = netboost::poll(&fds, 1, 5);

    return ret != 0;
}


int netboost::TcpSocket::send(void* data, size_t dataSize) noexcept
{
    return ::send(handle, (char*)data, dataSize, 0);
}

int netboost::TcpSocket::receive(void* buffer, int dataSize) noexcept
{
    int rres = ::recv(handle, (char*)buffer, dataSize, 0);

    if (rres <= 0)
        m_connected = false;

    return rres;
}

int netboost::TcpSocket::nativeHandle() const noexcept
{
    return handle;
}

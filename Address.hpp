#ifndef Adress_hpp
#define Adress_hpp

#include <string>

#include "netinit.hpp"

class Address
{
public:
    
    Address() = default;
    Address(std::string addressFull);
    Address(std::string address, unsigned short port);
    Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port);
    Address(unsigned int address, unsigned short port);

    unsigned int getAddress() const noexcept;
    std::string getAddress(bool addPort = false) const noexcept;
    sockaddr_in* getSockAddr() noexcept;
    unsigned char getA() const noexcept;
    unsigned char getB() const noexcept;
    unsigned char getC() const noexcept;
    unsigned char getD() const noexcept;
    unsigned short getPort() const noexcept;
    bool operator == ( const Address & other ) const noexcept;
    bool operator != ( const Address & other ) const noexcept;
    
private:
    void initSockAddr() noexcept;

    sockaddr_in sockaddrIn;
    unsigned int address;
    unsigned short port;
};

#endif /* Adress_hpp */

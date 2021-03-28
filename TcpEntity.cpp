#include "TcpEntity.hpp"

bool netboost::TcpEntity::active(netboost::ConnectionDescriptor descriptor)
{
    int index = descriptor - 1;
    if ((index < 0) || !(sockMask & (1ULL << index)))
        return false;
    return sockets.at(index)->connected();
}


bool netboost::TcpEntity::readable(netboost::ConnectionDescriptor descriptor)
{
    int index = descriptor - 1;
    return ((index < 0) || !(sockMask & (1ULL << index))) ? false : sockets.at(index)->readable();
}

//size_t netboost::TcpEntity::write(netboost::ConnectionDescriptor descriptor, void *data, int dataSize)
//{
//    int index = descriptor - 1;
//    if ((index < 0) || !(sockMask & (1ULL << index)))
//    {
//        return 0;
//    }
//    if (sockets.at(index)->send(&dataSize, sizeof(int)) == sizeof(int))
//    {
//        return sockets.at(index)->send(data, dataSize);
//    }
//    std::cout << "DROP!\n";
//    return 0;
//}

bool netboost::TcpEntity::write(ConnectionDescriptor descriptor, const Packet& packet)
{
    write(descriptor, packet.getSize(), sizeof(packet_size));
    size_t res = write(descriptor, *static_cast<const byte*>(packet.getData()), packet.getSize());

    return res == packet.getSize();
}

netboost::Packet netboost::TcpEntity::readPacket(ConnectionDescriptor descriptor)
{
    packet_size packetSize = 0;
    if (read(descriptor, &packetSize, sizeof(packet_size)) == -1)
        return false;

    void* recData = new byte[packetSize];
    if (read(descriptor, recData, packetSize) == -1)
        return false;

  /*  for (int i = 0; i < packetSize; i++)
    {
        std::cout << *((char*)recData + i) << std::endl;
    }*/

     return Packet(recData, packetSize, false);
}

int netboost::TcpEntity::read(netboost::ConnectionDescriptor descriptor, void *buffer, int bufferSize)
{
    int index = descriptor - 1;
    if ((index < 0) || (!(sockMask & (1ULL << index))))
        return 0;

    int len = 0;
    if (sockets.at(index)->receive(&len, sizeof(int)) == sizeof(int))
    {
        return sockets.at(index)->receive(buffer, bufferSize);
    }
    return -1;
}

void netboost::TcpEntity::disconnect(netboost::ConnectionDescriptor descriptor)
{
    int index = descriptor - 1;
    if ((sockMask & (1ULL << index)))
    {
        sockets.at(index)->disconnect();
        sockMask ^= (1ULL << index);
    }
}
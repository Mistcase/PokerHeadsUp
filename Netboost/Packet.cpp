#include "Packet.hpp"

netboost::Packet::Packet(packet_size size)
{
    this->size = size;
    ptr = packet_data_ptr(new char[size]);
}

netboost::Packet::Packet(const void* data, packet_size size, bool readOnly) : Packet(size)
{
    if (readOnly)
    {
        ptrReadOnly = data;
    }
    else
    {
        ptr = packet_data_ptr((char*)(data));
    }
}

uint32_t netboost::Packet::getSize() const
{
    return size;
}

const void* netboost::Packet::getData() const
{
    return ptrReadOnly != nullptr ? ptrReadOnly : ptr.get();
}
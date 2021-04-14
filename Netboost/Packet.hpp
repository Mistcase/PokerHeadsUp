#ifndef Packet_h
#define Packet_h

#include <sys/types.h>
#include <cstdint>
#include <memory>

typedef uint32_t packet_size;
typedef std::shared_ptr<void> packet_data_ptr;

namespace netboost
{
    //class TcpEntity;
    class Packet
    {
    public:
        Packet() = default;
        Packet(packet_size size);
        Packet(const void* data, packet_size size, bool readOnly = true);

        packet_size getSize() const;
        const void* getData() const;

    private:
        const void* ptrReadOnly = nullptr;

        packet_size size = 0;
        packet_data_ptr ptr = nullptr;

        //friend class TcpEntity;
    };
}

#endif

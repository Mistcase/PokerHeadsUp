#ifndef TcpClient_hpp
#define TcpClient_hpp

#include <array>

#include "Address.hpp"
#include "TcpEntity.hpp"
#include "TcpSocket.hpp"
#include "netinit.hpp"

namespace netboost
{
	class TcpClient : public TcpEntity
	{
	public:
		TcpClient() = default;
		ConnectionDescriptor connect(const Address& address);
	};

}

#endif

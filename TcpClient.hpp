#ifndef TcpClient_hpp
#define TcpClient_hpp

#include <array>

#include "netinit.hpp"
#include "TcpSocket.hpp"
#include "Address.hpp"
#include "TcpEntity.hpp"

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

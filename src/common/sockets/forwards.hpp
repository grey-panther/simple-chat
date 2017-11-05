#ifndef SIMPLE_CHAT_SOCKETS_FORWARDS_HPP
#define SIMPLE_CHAT_SOCKETS_FORWARDS_HPP

#include <memory>


namespace sockets
{
	class ISocketAddress;

	class IClientSocketTCP;
	typedef std::shared_ptr<IClientSocketTCP> IClientSocketTCPSPtr;

	class IServerSocketTCP;
	typedef std::shared_ptr<IServerSocketTCP> IServerSocketTCPSPtr;
}

#endif

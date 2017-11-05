#ifndef SIMPLE_CHAT_SOCKETFABRIC_HPP
#define SIMPLE_CHAT_SOCKETFABRIC_HPP

#include "sockets/forwards.hpp"
#include "Logger.hpp"


namespace sockets
{
	namespace factory
	{
		template<class T>
		T* make(const ISocketAddress&)
		{
			Logger::channel(ERR) << "Not implemented factory type!";
			return nullptr;
		}

		template<>
		IServerSocketTCP* make<IServerSocketTCP>(const ISocketAddress& address);

		template<>
		IClientSocketTCP* make<IClientSocketTCP>(const ISocketAddress& address);
	}
}

#endif

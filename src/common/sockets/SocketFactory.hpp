#ifndef SIMPLE_CHAT_SOCKETFACTORY_HPP
#define SIMPLE_CHAT_SOCKETFACTORY_HPP

#include "sockets/forwards.hpp"
#include "Logger.hpp"


namespace sockets
{
	namespace factory
	{
		template<class T>
		T* make(const ITasksQueueSPtr& tasks_queue, const ISocketAddress&)
		{
			Logger::channel(ERR) << "Not implemented factory type!";
			return nullptr;
		}

		template<>
		IServerSocketTCP* make<IServerSocketTCP>(const ITasksQueueSPtr& tasks_queue, const ISocketAddress& address);

		template<>
		IClientSocketTCP* make<IClientSocketTCP>(const ITasksQueueSPtr& tasks_queue, const ISocketAddress& address);
	}
}

#endif

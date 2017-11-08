#ifndef SIMPLE_CHAT_SOCKETFACTORY_HPP
#define SIMPLE_CHAT_SOCKETFACTORY_HPP

#include "sockets/forwards.hpp"
#include "Logger.hpp"


namespace sockets
{
	namespace factory
	{
		template<class T>
		T* make(TasksProcessor& task_processor, const ISocketAddress&)
		{
			Logger::channel(ERR) << "Not implemented factory type!";
			return nullptr;
		}

		template<>
		IServerSocketTCP* make<IServerSocketTCP>(TasksProcessor& task_processor, const ISocketAddress& address);

		template<>
		IClientSocketTCP* make<IClientSocketTCP>(TasksProcessor& task_processor, const ISocketAddress& address);
	}
}

#endif

#ifndef SIMPLE_CHAT_SOCKETS_FORWARDS_HPP
#define SIMPLE_CHAT_SOCKETS_FORWARDS_HPP

#include <memory>


namespace sockets
{
	class ISocket;
	typedef std::weak_ptr<ISocket> ISocketWPtr;

	class ISocketAddress;
	typedef std::shared_ptr<ISocketAddress> ISocketAddressSPtr;

	class IClientSocketTCP;
	typedef std::shared_ptr<IClientSocketTCP> IClientSocketTCPSPtr;

	class IServerSocketTCP;
	typedef std::shared_ptr<IServerSocketTCP> IServerSocketTCPSPtr;
	typedef std::weak_ptr<IServerSocketTCP> IServerSocketTCPWPtr;

	class ISocketUDP;
	typedef std::shared_ptr<ISocketUDP> ISocketUDPSPtr;

	class TasksProcessor;
	typedef std::shared_ptr<TasksProcessor> TasksProcessorSPtr;

	class ITasksQueue;
	typedef std::shared_ptr<ITasksQueue> ITasksQueueSPtr;
}

#endif

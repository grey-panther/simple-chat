#include "sockets/SocketFactory.hpp"
#include "sockets/ServerSocketTCP.hpp"
#include "sockets/ClientSocketTCP.hpp"


namespace sockets
{
	namespace factory
	{
		template<>
		IServerSocketTCP* make<IServerSocketTCP>(const ITasksQueueSPtr& tasks_queue, const ISocketAddress& address)
		{
			IServerSocketTCP* socket = new ServerSocketTCP(tasks_queue);
			socket->set_address(address);
			return socket;
		}

		template<>
		IClientSocketTCP* make<IClientSocketTCP>(const ITasksQueueSPtr& tasks_queue, const ISocketAddress& address)
		{
			return new ClientSocketTCP(tasks_queue);
		}
	}
}

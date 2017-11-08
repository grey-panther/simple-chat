#include "sockets/SocketFactory.hpp"
#include "sockets/ServerSocketTCP.hpp"
#include "sockets/ClientSocketTCP.hpp"


namespace sockets
{
	namespace factory
	{
		template<>
		IServerSocketTCP* make<IServerSocketTCP>(TasksProcessor& task_processor, const ISocketAddress& address)
		{
			IServerSocketTCP* socket = new ServerSocketTCP(task_processor);
			socket->set_address(address);
			return socket;
		}

		template<>
		IClientSocketTCP* make<IClientSocketTCP>(TasksProcessor& task_processor, const ISocketAddress& address)
		{
			return new ClientSocketTCP(task_processor);
		}


//		template<>
//		IServerSocketTCP* make<IServerSocketTCP>()
//		{
//			return new ServerSocketTCP;
//		}
	}
}
#include "sockets/SocketFactory.hpp"
#include "sockets/ServerSocketTCP.hpp"
#include "sockets/ClientSocketTCP.hpp"


namespace sockets
{
	namespace factory
	{
		template<>
		IServerSocketTCP* make<IServerSocketTCP>(const ISocketAddress& address)
		{
			IServerSocketTCP* socket = new ServerSocketTCP;
			socket->set_address(address);
			return socket;
		}

		template<>
		IClientSocketTCP* make<IClientSocketTCP>(const ISocketAddress& address)
		{
			return new ClientSocketTCP;
		}


//		template<>
//		IServerSocketTCP* make<IServerSocketTCP>()
//		{
//			return new ServerSocketTCP;
//		}
	}
}

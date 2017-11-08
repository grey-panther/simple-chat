#include "ChatServer.hpp"
#include "sockets/SocketAddress.hpp"


int main()
{
	ChatServer server(sockets::SocketAddress("127.0.0.1", 4242));
	server.run();

	return 0;
}
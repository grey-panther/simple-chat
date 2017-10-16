#include <ISocket.hpp>
#include <Logger.hpp>
#include <Socket.hpp>
#include <SocketAddress.hpp>
#include <memory>


typedef std::shared_ptr<ISocket> ISocketSPtr;


int main()
{
	Logger::channel(INFO) << "Hello";

	ISocketSPtr socket(new Socket);
	SocketAddress addr_1("127.0.0.1", 4242);
	socket->set_address(addr_1);

	ISocketSPtr other_socket(new Socket);
	SocketAddress addr_2("127.0.0.1", 4243);
	other_socket->set_address(addr_2);

	socket->send_to("foobar", addr_2);
	other_socket->receive();

	return 0;
}

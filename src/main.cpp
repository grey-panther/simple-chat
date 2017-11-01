#include "sockets/ISocket.hpp"
#include "sockets/SocketAddress.hpp"
#include "sockets/ClientSocketTCP.hpp"
#include "sockets/SocketUDP.hpp"
#include "sockets/ServerSocketTCP.hpp"
#include "Logger.hpp"
#include <memory>


typedef std::shared_ptr<sockets::ISocket> ISocketSPtr;
typedef std::shared_ptr<sockets::ISocketUDP> ISocketUDPSPtr;
typedef std::shared_ptr<sockets::IClientSocketTCP> IClientSocketTCPSPtr;
typedef std::shared_ptr<sockets::IServerSocketTCP> IServerSocketTCPSPtr;

using namespace sockets;

void test_udp();

void test_tcp();


int main()
{
	test_udp();
	test_tcp();

	return 0;
}


void test_udp()
{
	ISocketUDPSPtr socket(new SocketUDP);
	SocketAddress addr_1("127.0.0.1", 4242);
	socket->set_address(addr_1);

	ISocketUDPSPtr other_socket(new SocketUDP);
	SocketAddress addr_2("127.0.0.1", 4243);
	other_socket->set_address(addr_2);

	const std::string sending_msg = "foobar";
	Logger::channel(INFO) << "Sending UDP message: " << sending_msg;

	socket->send_to(sending_msg, addr_2);
	std::string received_msg = other_socket->receive();

	Logger::channel(INFO) << "Received UDP message: " << received_msg;

	if (sending_msg != received_msg) {
		Logger::channel(ERR) << "Received message not equal to sending message!";
		return;
	}
	Logger::channel(INFO) << "UDP test OK";
}


void test_tcp()
{
	IServerSocketTCPSPtr connections_listener(new ServerSocketTCP);
	SocketAddress server_addr = SocketAddress("127.0.0.1", 4246);
	connections_listener->set_address(server_addr);

	IClientSocketTCPSPtr server_last_client;    	// Last client which was accepted by server
	connections_listener->listen(
			[&server_last_client](IClientSocketTCPSPtr connected_socket) {
				server_last_client = connected_socket;
			}
	);

	IClientSocketTCPSPtr some_client(new ClientSocketTCP);
	some_client->connect(server_addr);

	while(!server_last_client);				// Wait until server accept connection

	const std::string sending_msg = "foobar tcp";
	Logger::channel(INFO) << "Sending TCP message: " << sending_msg;

	some_client->send(sending_msg);
	std::string received_msg = server_last_client->receive();

	Logger::channel(INFO) << "Received TCP message: " << received_msg;

	if (sending_msg != received_msg) {
		Logger::channel(ERR) << "Received message not equal to sending message!";
		return;
	}
	Logger::channel(INFO) << "TCP test OK";
}

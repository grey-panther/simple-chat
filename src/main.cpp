#include "ISocket.hpp"
#include "SocketAddress.hpp"
#include "SocketTCP.hpp"
#include "SocketUDP.hpp"
#include "Logger.hpp"
#include <memory>


typedef std::shared_ptr<ISocket> ISocketSPtr;
typedef std::shared_ptr<ISocketUDP> ISocketUDPSPtr;
typedef std::shared_ptr<ISocketTCP> ISocketTCPSPtr;


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
	ISocketTCPSPtr connections_listener(new SocketTCP);
	SocketAddress server_addr = SocketAddress("127.0.0.1", 4244);
	connections_listener->set_address(server_addr);

	ISocketTCPSPtr server_last_client;    	// Last client which was accepted by server
	connections_listener->listen(
			[&server_last_client](ISocketTCPSPtr connected_socket) {
				server_last_client = connected_socket;
			}
	);

	ISocketTCPSPtr some_client(new SocketTCP);
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

#include "sockets/ISocket.hpp"
#include "sockets/SocketAddress.hpp"
#include "sockets/ClientSocketTCP.hpp"
#include "sockets/SocketUDP.hpp"
#include "sockets/ServerSocketTCP.hpp"
#include "ChatServer.hpp"
#include "ChatMessage.hpp"
#include "Logger.hpp"
#include <memory>
#include "ChatClient.hpp"


typedef std::shared_ptr<sockets::ISocketUDP> ISocketUDPSPtr;

using namespace sockets;

void test_udp();

void test_tcp();

void test_chat();

int main()
{
//	test_udp();
//	test_tcp();

	test_chat();

	return 0;
}


void test_udp()
{
	TasksProcessor _tasks_processor;
	ISocketUDPSPtr socket(new SocketUDP(_tasks_processor));
	SocketAddress addr_1("127.0.0.1", 4242);
	socket->set_address(addr_1);

	ISocketUDPSPtr other_socket(new SocketUDP(_tasks_processor));
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
	TasksProcessor _tasks_processor;
	IServerSocketTCPSPtr connections_listener(new ServerSocketTCP(_tasks_processor));
	SocketAddress server_addr = SocketAddress("127.0.0.1", 4246);
	connections_listener->set_address(server_addr);

	IClientSocketTCPSPtr server_last_client;        // Last client which was accepted by server
	connections_listener->listen(
			[&server_last_client](IClientSocketTCPSPtr connected_socket) {
				server_last_client = connected_socket;
			}
	);

	IClientSocketTCPSPtr some_client(new ClientSocketTCP(_tasks_processor));
	some_client->connect(server_addr, [] (const bool) {});

	while (!server_last_client);                // Wait until server accept connection

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

void test_chat()
{
	ISocketAddressSPtr server_addr = std::make_shared<SocketAddress>("127.0.0.1", 4242);
	ChatServer server(*server_addr);
	std::thread s([&server] { server.run(); });

	ChatClient client1(
			server_addr,
			[](const ChatMessageSPtr& message) {
				Logger::channel(INFO) << "Client 1 got message: " << message->data();
			},
			[]() {
				Logger::channel(INFO) << "Client 1 closed connection.";
			}
	);
	std::thread c1([&client1] { client1.run(); });

	ChatClient client2(
			server_addr,
			[](const ChatMessageSPtr& message) {
				Logger::channel(INFO) << "Client 2 got message: " << message->data();
			},
			[]() {
				Logger::channel(INFO) << "Client 2 closed connection.";
			}
	);
	std::thread c2([&client2] { client2.run(); });

	// Wait for connection
	while (!client1.connected() || !client2.connected());
	ChatMessageSPtr msg(new ChatMessage("Foo"));
	client1.send(msg);
	ChatMessageSPtr msg2(new ChatMessage("Bar"));
	client2.send(msg2);

//	std::this_thread::sleep_for(std::chrono::seconds(2));
//	client1.stop();
//	c1.join();
//
//	std::this_thread::sleep_for(std::chrono::seconds(2));
//	client2.stop();
//	c2.join();

	std::this_thread::sleep_for(std::chrono::seconds(5));
	Logger::channel(INFO) << "DESTRUCTION";
	server.stop();
	s.join();

	c1.join();
	c2.join();

	// TODO Compare strings that were sent and received.
}
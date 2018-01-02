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

	auto on_receive_msg = [](const int client_index, std::string* out_msg, const ChatMessageSPtr& message) {
		Logger::channel(INFO) << "Client " << client_index << " got message: " << message->data();
		*out_msg = std::string(message->data());
	};
	auto on_close_connection = [](const int client_index) {
		Logger::channel(INFO) << "Client " << client_index << " closed connection.";
	};

	std::string* cl1_received_message = new std::string("empty1");
	ChatClient client1(
			server_addr,
			std::bind(on_receive_msg, 1, cl1_received_message, std::placeholders::_1),
			std::bind(on_close_connection, 1)
	);
	std::thread c1([&client1] { client1.run(); });

	std::string* cl2_received_message = new std::string("empty2");
	ChatClient client2(
			server_addr,
			std::bind(on_receive_msg, 2, cl2_received_message, std::placeholders::_1),
			std::bind(on_close_connection, 2)
	);
	std::thread c2([&client2] { client2.run(); });

	// Wait for clients connection
	while (!client1.connected() || !client2.connected());

	// Send messages
	const std::string cl1_sending_message = "Foo";
	ChatMessageSPtr msg(new ChatMessage(cl1_sending_message));
	client1.send(msg);

	const std::string cl2_sending_message = "Bar";
	ChatMessageSPtr msg2(new ChatMessage(cl2_sending_message));
	client2.send(msg2);

//	std::this_thread::sleep_for(std::chrono::seconds(2));
//	client1.stop();
//	c1.join();
//
//	std::this_thread::sleep_for(std::chrono::seconds(2));
//	client2.stop();
//	c2.join();

	std::this_thread::sleep_for(std::chrono::seconds(5));
	Logger::channel(INFO) << "STOPPING SERVER...";
	server.stop();
	s.join();

	c1.join();
	c2.join();

	// Check sent and received strings
	if (cl1_sending_message != *cl2_received_message) {
		Logger::channel(ERR) << "cl1_sending_message != cl2_received_message :\n"
							 << "cl1_sending_message = " << cl1_sending_message << "\n"
							 << "cl2_received_message = " << *cl2_received_message;
	}
	if (cl2_sending_message != *cl1_received_message) {
		Logger::channel(ERR) << "cl2_sending_message != cl1_received_message :\n"
							 << "cl2_sending_message = " << cl2_sending_message << "\n"
							 << "cl1_received_message = " << *cl1_received_message;
	}

	delete cl1_received_message;
	delete cl2_received_message;
}
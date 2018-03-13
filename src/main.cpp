#include "sockets/ISocket.hpp"
#include "sockets/SocketAddress.hpp"
#include "sockets/ClientSocketTCP.hpp"
#include "sockets/SocketUDP.hpp"
#include "sockets/ServerSocketTCP.hpp"
#include "sockets/TasksProcessor.hpp"
#include "ChatClient.hpp"
#include "ChatServer.hpp"
#include "ChatMessage.hpp"
#include "Logger.hpp"
#include <vector>


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
	TasksProcessorSPtr _tasks_processor(new TasksProcessor);
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
	TasksProcessorSPtr tasks(new TasksProcessor);
	std::thread tasks_thread([tasks] { tasks->run(); });

	// Create a connections listener (a server socket).
	IServerSocketTCPSPtr connections_listener(new ServerSocketTCP(tasks));
	SocketAddress server_addr = SocketAddress("127.0.0.1", 4246);
	connections_listener->set_address(server_addr);

	// Create a server side socket;
	IClientSocketTCPSPtr server_side_socket;
	connections_listener->listen(
			[&server_side_socket](IClientSocketTCPSPtr connected_socket) {
				server_side_socket = connected_socket;
			}
	);

	// Create a client side socket and make a connection to the server.
	IClientSocketTCPSPtr client_socket(new ClientSocketTCP(tasks));
	client_socket->connect(server_addr, [] (const bool) {});
	// Wait until the server accept connection.
	while (!server_side_socket);

	// Send a message through socket.
	const std::string sending_msg = "foobar tcp";
	Logger::channel(INFO) << "Sending TCP message: " << sending_msg;
	client_socket->write(sending_msg.data(), sending_msg.size(), [](bool) {});

	// Receive the message on server side.
	std::vector<char> received_msg_data(sending_msg.size());
	bool reading_complete = false;
	server_side_socket->read(received_msg_data.data(), received_msg_data.size(), [&reading_complete](bool) {
		reading_complete = true;
	});
	// Wait until message is read from socket.
	while (!reading_complete);

	const std::string received_msg(received_msg_data.cbegin(), received_msg_data.cend());
	Logger::channel(INFO) << "Received TCP message: " << received_msg;

	// Check that sent and received messages are equal.
	if (sending_msg == received_msg) {
		Logger::channel(INFO) << "TCP test OK";
	} else {
		Logger::channel(ERR) << "Received message not equal to sending message!";
	}

	tasks->stop();
	tasks_thread.join();
}


void test_chat()
{
	ISocketAddressSPtr server_addr = std::make_shared<SocketAddress>("127.0.0.1", 4242);
	ChatServer server(*server_addr);
	std::thread s([&server] { server.run(); });

	typedef std::shared_ptr<std::string> string_sptr;
	auto on_receive_msg = [](const int client_index, string_sptr out_msg, const ChatMessageSPtr& message) {
		Logger::channel(INFO) << "Client " << client_index << " got message: " << message->data();
		*out_msg = std::string(message->data());
	};
	auto on_close_connection = [](const int client_index) {
		Logger::channel(INFO) << "Client " << client_index << " closed connection.";
	};

	string_sptr cl1_received_message = std::make_shared<std::string>("message1");
	ChatClient client1(
			server_addr,
			std::bind(on_receive_msg, 1, cl1_received_message, std::placeholders::_1),
			std::bind(on_close_connection, 1)
	);
	std::thread c1([&client1] { client1.run(); });

	string_sptr cl2_received_message = std::make_shared<std::string>("message2");
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
	const bool is_cl1_message_ok = cl1_sending_message == *cl2_received_message;
	if (!is_cl1_message_ok) {
		Logger::channel(ERR) << "cl1_sending_message != cl2_received_message :\n"
							 << "cl1_sending_message = " << cl1_sending_message << "\n"
							 << "cl2_received_message = " << *cl2_received_message;
	}
	const bool is_cl2_message_ok = cl2_sending_message == *cl1_received_message;
	if (!is_cl2_message_ok) {
		Logger::channel(ERR) << "cl2_sending_message != cl1_received_message :\n"
							 << "cl2_sending_message = " << cl2_sending_message << "\n"
							 << "cl1_received_message = " << *cl1_received_message;
	}
	if (is_cl1_message_ok && is_cl2_message_ok) {
		Logger::channel(INFO) << "TEST OK";
	} else {
		Logger::channel(ERR) << "TEST FAILED";
	}
}
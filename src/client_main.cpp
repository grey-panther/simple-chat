#include "sockets/forwards.hpp"
#include "sockets/SocketAddress.hpp"
#include "forwards.hpp"
#include "ChatClient.hpp"
#include "ChatMessage.hpp"
#ifdef WIN32
#include <mingw.thread.h>
#else
#include <thread>
#endif


void on_message_received(const ChatMessageSPtr& message)
{
	std::cout << std::endl << "Someone wrote: " << message->data() << std::endl;
}

void on_connection_closed()
{
	std::cout << std::endl << "CONNECTION CLOSED" << std::endl;
}

int main()
{
	Logger::set_enabled(false);

	sockets::ISocketAddressSPtr server_addr = std::make_shared<sockets::SocketAddress>("127.0.0.1", 4242);
	ChatClient client(server_addr, on_message_received, on_connection_closed);

	std::thread client_thread([&client] { client.run(); });

	std::string input;
	while (getline(std::cin, input) && client.connected()) {
		std::cout << std::endl << "You wrote: " << input << std::endl;
		client.send(ChatMessageSPtr(new ChatMessage(input)));
	}

	client_thread.join();
	return 0;
}

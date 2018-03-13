#ifndef SIMPLE_CHAT_CHATCLIENT_HPP
#define SIMPLE_CHAT_CHATCLIENT_HPP

#include "forwards.hpp"
#include "sockets/forwards.hpp"
#include <atomic>


class ChatClient
{
	typedef std::function<void(const ChatMessageSPtr& message)> on_message_received_t;
	typedef std::function<void()> on_connection_closed_t;

	sockets::TasksProcessorSPtr _sockets_tasks_processor;
	sockets::IClientSocketTCPSPtr _socket;
	sockets::ISocketAddressSPtr _server_address;
	on_message_received_t _on_message_received;
	on_connection_closed_t _on_connection_closed;
	std::atomic<bool> _is_connected;

public:
	ChatClient(
			const sockets::ISocketAddressSPtr& server_address,
			const on_message_received_t& on_message_received,
			const on_connection_closed_t& on_connection_closed = on_connection_closed_t()
	);

	void run();

	void stop();

	void send(const ChatMessageSPtr& message);

	inline bool connected() const
	{ return _is_connected; }

private:
	void do_receive();
};


#endif

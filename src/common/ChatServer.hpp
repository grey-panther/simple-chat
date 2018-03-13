#ifndef SIMPLE_CHAT_CHATSERVER_HPP
#define SIMPLE_CHAT_CHATSERVER_HPP

#include "forwards.hpp"
#include "sockets/forwards.hpp"


class ChatServer
{
	ChatRoomSPtr _chat_room;
	sockets::TasksProcessorSPtr _sockets_tasks_processor;
	sockets::IServerSocketTCPSPtr _connections_listener;

public:
	explicit ChatServer(const sockets::ISocketAddress& address);

	void run();

	void stop();
};


#endif

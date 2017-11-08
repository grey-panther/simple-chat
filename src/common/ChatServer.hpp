#ifndef SIMPLE_CHAT_CHATSERVER_HPP
#define SIMPLE_CHAT_CHATSERVER_HPP

#include "forwards.hpp"
#include "sockets/forwards.hpp"
#include "sockets/IServerSocketTCP.hpp"
#include "sockets/SocketFactory.hpp"
#include "ChatRoom.hpp"


class ChatServer
{
	ChatRoomSPtr _chat_room;
	sockets::TasksProcessor _sockets_tasks_processor;
	sockets::IServerSocketTCPSPtr _connections_listener;

public:
	ChatServer(const sockets::ISocketAddress& address);

	void run();

	void stop();
};


#endif

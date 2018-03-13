#include "ChatServer.hpp"
#include "ChatParticipant.hpp"
#include "ChatRoom.hpp"
#include "sockets/IServerSocketTCP.hpp"
#include "sockets/SocketFactory.hpp"
#include "sockets/TasksProcessor.hpp"


ChatServer::ChatServer(const sockets::ISocketAddress& address)
		: _sockets_tasks_processor(new sockets::TasksProcessor)
		, _chat_room(new ChatRoom)
{
	_connections_listener.reset(sockets::factory::make<sockets::IServerSocketTCP>(_sockets_tasks_processor, address));
}


void ChatServer::run()
{
	if (!_connections_listener) {
		Logger::channel(WARN) << "Cannot run server: listening socket is not created";
		return;
	}

	// Listen connections. When connection comes, create participant and start its session.
	_connections_listener->listen([this](sockets::IClientSocketTCPSPtr connected_socket) {
		Logger::channel(INFO) << "ChatServer: Connection is accepted";
		std::make_shared<ChatParticipant>(connected_socket, *_chat_room)->start_session();
	});

	_sockets_tasks_processor->run();
}


void ChatServer::stop()
{
	_sockets_tasks_processor->stop();
	_connections_listener.reset();
	_chat_room.reset();
}

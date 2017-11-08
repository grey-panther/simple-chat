#include "ChatServer.hpp"
#include "ChatParticipant.hpp"


ChatServer::ChatServer(const sockets::ISocketAddress& address)
		: _chat_room(new ChatRoom)
{
	_connections_listener.reset(sockets::factory::make<sockets::IServerSocketTCP>(_sockets_tasks_processor, address));
}


void ChatServer::run()
{
	if (!_connections_listener || _sockets_tasks_processor.terminated()) {
		Logger::channel(WARN) << "Cannot run server: terminated before start.";
		return;
	}

	// Слушать коннекшны. Приходит коннекшт - создавать участника и стартовать его сессию.
	_connections_listener->listen([this] (sockets::IClientSocketTCPSPtr connected_socket) {
		Logger::channel(INFO) << "ChatServer: Connection is accepted";
		std::make_shared<ChatParticipant>(connected_socket, *_chat_room)->start_session();
	});

	_sockets_tasks_processor.run();
}


void ChatServer::stop()
{
	_sockets_tasks_processor.stop();
	_connections_listener.reset();
	_chat_room.reset();
}

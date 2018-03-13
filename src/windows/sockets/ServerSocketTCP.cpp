#include "sockets/ServerSocketTCP.hpp"
#include "sockets/ClientSocketTCP.hpp"
#include "Logger.hpp"


namespace sockets
{
	ServerSocketTCP::ServerSocketTCP(const ITasksQueueSPtr& tasks_queue)
			: SocketBase(tasks_queue, SOCK_STREAM)
			, _is_listening(false)
	{
	}


	ServerSocketTCP::~ServerSocketTCP()
	{
		// Join listening thread.
		if (_listening_thread) {
			_is_listening = false;
			_listening_thread->join();
			Logger::channel(INFO) << "ServerSocketTCP::~ServerSocketTCP()" << " listen_thread joined";
			_listening_thread.reset();
		}
	}


	void ServerSocketTCP::listen(const IServerSocketTCP::on_accept_connection_callback_t& on_accept_connection)
	{
		if (_is_listening) {
			Logger::channel(WARN) << "Socket is already listening";
			return;
		}

		static const int CONNECTIONS_QUEUE_LENGTH = 10;
		const int result = ::listen(socket(), CONNECTIONS_QUEUE_LENGTH);

		if (result == SOCKET_ERROR) {
			handle_error(SocketErrorGroup::LISTEN, WSAGetLastError());
			return;
		}

		_is_listening = true;
		_listening_thread.reset(new std::thread(std::bind(&ServerSocketTCP::do_listen, this, on_accept_connection)));
	}


	void ServerSocketTCP::do_listen(const IServerSocketTCP::on_accept_connection_callback_t& on_accept_connection)
	{
		fd_set read_set;

		while (_is_listening) {
			static const timeval POLL_TIMEOUT_SEC { 5 };
			FD_ZERO(&read_set);
			FD_SET(socket(), &read_set);

			// Wait until the socket be ready to read, but no longer than POLL_TIMEOUT_SEC seconds.
			const int select_result = ::select(0, &read_set, nullptr, nullptr, &POLL_TIMEOUT_SEC);

			if (select_result == SOCKET_ERROR) {
				handle_error(SocketErrorGroup::SELECT, WSAGetLastError());
				return;
			}

			// If timeout has happened or it isn't expected socket to read.
			const bool is_timeout = select_result == 0;
			if (is_timeout || !(FD_ISSET(socket(), &read_set))) {
				continue;
			}

			// Do accept() immediately, because the socket has been ready to read.
			const SOCKET socket_handle = ::accept(socket(), nullptr, nullptr);
			if (socket_handle == SOCKET_ERROR) {
				handle_error(SocketErrorGroup::ACCEPT, WSAGetLastError());
				return;
			}

			Logger::channel(INFO) << "ServerSocketTCP::do_listen()" << " Call on_accept_connection().";
			std::shared_ptr<IClientSocketTCP> accepted_socket(new ClientSocketTCP(tasks(), socket_handle));
			execute_callback(std::bind(on_accept_connection, accepted_socket));
		}
	}
}

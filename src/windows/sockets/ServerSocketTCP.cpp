#include "sockets/ServerSocketTCP.hpp"
#include "sockets/ClientSocketTCP.hpp"
#include "Logger.hpp"


namespace sockets
{
	ServerSocketTCP::~ServerSocketTCP()
	{
		_is_listening = false;

		if (_listening_thread) {
			_listening_thread->join();
		}
	}


	void ServerSocketTCP::listen(const IServerSocketTCP::on_accept_connection_callback_t& on_accept_connection)
	{
		if (_is_listening) {
			// TODO Maybe it has sense to make method stop_listening();
			Logger::channel(WARN) << "Socket is already listening";
			return;
		}

		static const int CONNECTIONS_QUEUE_LENGTH = 10;
		const int result = ::listen(_socket, CONNECTIONS_QUEUE_LENGTH);

		if (result == SOCKET_ERROR) {
			handle_error(SocketErrorGroup::LISTEN, WSAGetLastError());
		}

		_is_listening = true;
		_listening_thread.reset(new std::thread(std::bind(&ServerSocketTCP::do_listening, this, on_accept_connection)));
	}


	void ServerSocketTCP::do_listening(const IServerSocketTCP::on_accept_connection_callback_t& on_accept_connection)
	{
		fd_set read_set;

		while (_is_listening) {
			static const timeval POLL_TIMEOUT_SEC { 10 };
			FD_ZERO(&read_set);
			FD_SET(_socket, &read_set);

			// Wait until the socket be ready to read, but no longer than POLL_TIMEOUT_SEC seconds.
			const int select_result = ::select(0, &read_set, nullptr, nullptr, &POLL_TIMEOUT_SEC);

			if (select_result == SOCKET_ERROR) {
				handle_error(SocketErrorGroup::SELECT, WSAGetLastError());
				return;
			}

			// If timeout has happened or it isn't expected socket to read.
			const bool is_timeout = select_result == 0;
			if (is_timeout || !(FD_ISSET(_socket, &read_set))) {
				continue;
			}

			// Do accept() immediately, because the socket has been ready to read.
			const SOCKET socket_handle = ::accept(_socket, nullptr, nullptr);
			if (socket_handle == SOCKET_ERROR) {
				handle_error(SocketErrorGroup::ACCEPT, WSAGetLastError());
				return;
			}

			std::shared_ptr<IClientSocketTCP> accepted_socket(new ClientSocketTCP(socket_handle));

			// TODO Вероятно, необходимо вызывать колбэк в главном потоке.
			// (смотреть std::mutex, https://habrahabr.ru/post/182610/).
			on_accept_connection(accepted_socket);
		}
	}
}

#include "ServerSocketTCP.hpp"
#include "Logger.hpp"
#include "ClientSocketTCP.hpp"
#include <netinet/in.h>


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

		if (result == -1) {
			handle_error(SocketErrorGroup::LISTEN, errno);
		}

		_is_listening = true;

		_socket_poll_data.events = POLLIN;
		_socket_poll_data.fd = _socket;

		_listening_thread.reset(new std::thread(std::bind(&ServerSocketTCP::do_listening, this, on_accept_connection)));
	}


	void ServerSocketTCP::do_listening(const IServerSocketTCP::on_accept_connection_callback_t& on_accept_connection)
	{
		while (_is_listening) {
			// Wait until POLLIN event happens, but no longer than POLL_TIMEOUT_SEC seconds.
			static const std::size_t POLL_TIMEOUT_SEC = 10;
			const int poll_result = ::poll(&_socket_poll_data, 1, POLL_TIMEOUT_SEC);

			if (poll_result == -1) {
				handle_error(SocketErrorGroup::POLL, errno);
				return;
			}

			// If timeout has happened.
			if (poll_result == 0) {
				continue;
			}

			// Do accept() immediately, because POLLIN event has happened.
			const int socket_handle = ::accept(_socket, nullptr, nullptr);
			if (socket_handle == -1) {
				handle_error(SocketErrorGroup::ACCEPT, errno);
				return;
			}

			std::shared_ptr<IClientSocketTCP> accepted_socket(new ClientSocketTCP(socket_handle));

			// TODO Use tasks queue to call callbacks.
			on_accept_connection(accepted_socket);
		}
	}
}

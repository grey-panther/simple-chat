#include "sockets/SocketAddress.hpp"
#include "sockets/ClientSocketTCP.hpp"
#include "Logger.hpp"


namespace sockets
{
	ClientSocketTCP::~ClientSocketTCP()
	{
		// Цепочка, приводящая к ошибке: sockets::ClientSocketTCP::do_read
		// -> ChatParticipant::<lambda(bool)>::operator()(bool)
		// -> ChatParticipant::~ChatParticipant
		// Может колбэки создавать в новом потоке, например с помощью асинк функций.
		// Тогда код внутри потока сам себя убивать не будет. Но, также придётся обратить внимание на метод read.
		// Теперь read никогда не будет запускаться из потока _read_thread
//		if (_read_thread) {
		if (_read_thread) {
			_is_reading = false;
			_read_thread->join();
			Logger::channel(INFO) << "ClientSocketTCP::~ClientSocketTCP()" << " read_thread joined";
			_read_thread.reset();
		}
	}


	void ClientSocketTCP::connect(const ISocketAddress& server_address, const on_complete_callback_t& on_complete)
	{
		int result = ::connect(_socket, server_address.inet_sockaddr(), server_address.inet_sockaddr_size());
		if (result == SOCKET_ERROR) {
			handle_error(SocketErrorGroup::CONNECT, WSAGetLastError(), server_address);
			tasks().add_task(ISocketWPtr(), std::bind(on_complete, true));
			return;
		}
		tasks().add_task(ISocketWPtr(), std::bind(on_complete, false));
	}


	void ClientSocketTCP::read(char* data, const std::size_t size,
							   const IClientSocketTCP::on_complete_callback_t& on_complete)
	{
		if (_is_reading) {
			Logger::channel(WARN) << "Socket is already reading.";
			return;
		}

		if (data == nullptr) {
			Logger::channel(ERR) << "Pointer to data must be not null";
			return;
		}

		if (_read_thread) {
			_read_thread->join();
		}

		_is_reading = true;

		// Create a new thread for reading.
		_read_thread.reset(new std::thread(&ClientSocketTCP::do_read, this, data, size, on_complete));
	}


	void ClientSocketTCP::write(const char* data, const std::size_t size,
								const IClientSocketTCP::on_complete_callback_t& on_complete) const
	{
		ssize_t result = ::send(_socket, data, size, 0);
		if (result == SOCKET_ERROR) {
			handle_error(SocketErrorGroup::SEND, WSAGetLastError());
			tasks().add_task(ISocketWPtr(), std::bind(on_complete, true));
			return;
		}
		tasks().add_task(ISocketWPtr(), std::bind(on_complete, false));
	}


	void ClientSocketTCP::do_read(char* data, const std::size_t data_size,
								  const IClientSocketTCP::on_complete_callback_t& on_complete)
	{
		fd_set read_set;

		while (_is_reading) {
			static const timeval POLL_TIMEOUT_SEC { 2 };
			FD_ZERO(&read_set);
			FD_SET(_socket, &read_set);

			// Wait until the socket be ready to read, but no longer than POLL_TIMEOUT_SEC seconds.
			const int select_result = ::select(0, &read_set, nullptr, nullptr, &POLL_TIMEOUT_SEC);

			if (select_result == SOCKET_ERROR) {
				handle_error(SocketErrorGroup::SELECT, WSAGetLastError());
				break;
			}

			// If timeout has happened or it isn't expected socket to read.
			const bool is_timeout = select_result == 0;
			if (is_timeout || !(FD_ISSET(_socket, &read_set))) {
				continue;
			}

			// Do recv() immediately, because the socket has been ready to read.
			ssize_t received_size = ::recv(_socket, data, data_size, 0);
			// TODO: Need do receiving through a loop, because current realization can miss trailing bytes of message.
			if (received_size == SOCKET_ERROR) {
				handle_error(SocketErrorGroup::RECEIVE, WSAGetLastError());
				tasks().add_task(ISocketWPtr(), std::bind(on_complete, true));
				break;
			}

			Logger::channel(INFO) << "ClientSocketTCP::do_read()" << " Try to add task.";
			if (_is_reading) {
				_is_reading = false;
				const bool is_connection_closed = received_size == 0;
				tasks().add_task(ISocketWPtr(), std::bind(on_complete, is_connection_closed));
			}
		}

		_is_reading = false;
	}
}

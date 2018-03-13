#include "sockets/SocketAddress.hpp"
#include "sockets/ClientSocketTCP.hpp"
#include "Logger.hpp"


namespace sockets
{
	ClientSocketTCP::ClientSocketTCP(const ITasksQueueSPtr& tasks_queue)
			: SocketBase(tasks_queue, SOCK_STREAM)
			, _is_reading(false)
	{}


	ClientSocketTCP::ClientSocketTCP(const ITasksQueueSPtr& tasks_queue, SOCKET socket)
			: SocketBase(tasks_queue, socket)
			, _is_reading(false)
	{}


	ClientSocketTCP::~ClientSocketTCP()
	{
		if (_reading_thread) {
			_is_reading = false;
			_reading_thread->join();
			Logger::channel(INFO) << "ClientSocketTCP::~ClientSocketTCP()" << " read_thread joined";
			_reading_thread.reset();
		}
	}


	void ClientSocketTCP::connect(const ISocketAddress& server_address, const on_complete_callback_t& on_complete)
	{
		int result = ::connect(socket(), server_address.inet_sockaddr(), server_address.inet_sockaddr_size());
		if (result == SOCKET_ERROR) {
			handle_error(SocketErrorGroup::CONNECT, WSAGetLastError(), server_address);
			execute_callback(std::bind(on_complete, true));
			return;
		}
		execute_callback(std::bind(on_complete, false));
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

		if (_reading_thread) {
			_reading_thread->join();
		}

		_is_reading = true;

		// Create a new thread for reading.
		_reading_thread.reset(new std::thread(&ClientSocketTCP::do_read, this, data, size, on_complete));
	}


	void ClientSocketTCP::write(const char* data, const std::size_t size,
								const IClientSocketTCP::on_complete_callback_t& on_complete) const
	{
		ssize_t result = ::send(socket(), data, size, 0);
		if (result == SOCKET_ERROR) {
			handle_error(SocketErrorGroup::SEND, WSAGetLastError());
			execute_callback(std::bind(on_complete, true));
			return;
		}
		execute_callback(std::bind(on_complete, false));
	}


	void ClientSocketTCP::do_read(char* data, const std::size_t data_size,
								  const IClientSocketTCP::on_complete_callback_t& on_complete)
	{
		fd_set read_set;

		while (_is_reading) {
			static const timeval POLL_TIMEOUT_SEC { 2 };
			FD_ZERO(&read_set);
			FD_SET(socket(), &read_set);

			// Wait until the socket be ready to read, but no longer than POLL_TIMEOUT_SEC seconds.
			const int select_result = ::select(0, &read_set, nullptr, nullptr, &POLL_TIMEOUT_SEC);

			if (select_result == SOCKET_ERROR) {
				handle_error(SocketErrorGroup::SELECT, WSAGetLastError());
				break;
			}

			// If timeout has happened or it isn't expected socket to read.
			const bool is_timeout = select_result == 0;
			if (is_timeout || !(FD_ISSET(socket(), &read_set))) {
				continue;
			}

			// Do recv() immediately, because the socket has been ready to read.
			ssize_t received_size = ::recv(socket(), data, data_size, 0);
			// TODO: Do receiving through a loop, because current implementation can miss trailing bytes of message.
			if (received_size == SOCKET_ERROR) {
				handle_error(SocketErrorGroup::RECEIVE, WSAGetLastError());
				execute_callback(std::bind(on_complete, true));
				break;
			}

			Logger::channel(INFO) << "ClientSocketTCP::do_read()" << " Try to add task.";
			if (_is_reading) {
				_is_reading = false;
				const bool is_connection_closed = received_size == 0;
				execute_callback(std::bind(on_complete, is_connection_closed));
			}
		}

		_is_reading = false;
	}
}

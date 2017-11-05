#include "sockets/SocketAddress.hpp"
#include "sockets/ClientSocketTCP.hpp"
#include "Logger.hpp"


namespace sockets
{
	ClientSocketTCP::~ClientSocketTCP()
	{
		_is_reading = false;

		if (_reading_thread) {
			_reading_thread->join();
		}
	}


	void ClientSocketTCP::connect(const ISocketAddress& server_address)
	{
		int result = ::connect(_socket, server_address.inet_sockaddr(), server_address.inet_sockaddr_size());
		if (result == SOCKET_ERROR) {
			handle_error(SocketErrorGroup::CONNECT, WSAGetLastError(), server_address);
		}
	}


	void ClientSocketTCP::send(const std::string& message)
	{
		ssize_t result = ::send(_socket, message.c_str(), message.size(), 0);
		if (result == SOCKET_ERROR) {
			handle_error(SocketErrorGroup::SEND, WSAGetLastError());
		}
	}


	std::string ClientSocketTCP::receive() const
	{
		static const std::size_t RECEIVED_DATA_SIZE = 100;
		std::shared_ptr<char> data(new char[RECEIVED_DATA_SIZE]);

		ssize_t received_size = ::recv(_socket, data.get(), RECEIVED_DATA_SIZE, 0);
		// TODO: Realise receiving through a loop, because current realisation can miss trailing bytes of message.

		if (received_size == SOCKET_ERROR) {
			handle_error(SocketErrorGroup::RECEIVE, WSAGetLastError());
			return "";
		}

		return std::string(data.get(), static_cast<std::size_t>(received_size));
	}


	void ClientSocketTCP::read(char* data, const std::size_t size,
							   const IClientSocketTCP::on_complete_callback_t& on_complete)
	{
		if (_is_reading) {
			Logger::channel(WARN) << "Socket is already reading.";
			return;
		}

		if (data == nullptr) {
			Logger::channel(ERR) << "The not null pointer to data is required.";
			return;
		}

		_is_reading = true;
		_reading_thread.reset(new std::thread(std::bind(&ClientSocketTCP::do_reading, this, data, size, on_complete)));
	}


	void ClientSocketTCP::write(const char* data, const std::size_t size,
								const IClientSocketTCP::on_complete_callback_t& on_complete) const
	{
		ssize_t result = ::send(_socket, data, size, 0);
		if (result == SOCKET_ERROR) {
			handle_error(SocketErrorGroup::SEND, WSAGetLastError());
			return;
		}
		on_complete();
	}


	void ClientSocketTCP::do_reading(char* data, const std::size_t size,
									 const on_complete_callback_t& on_complete)
	{
		fd_set read_set;

		while (_is_reading) {
			static const timeval POLL_TIMEOUT_SEC { 10 };
			FD_ZERO(&read_set);
			FD_SET(_socket, &read_set);

			// Wait until the socket be ready to read, but no longer than POLL_TIMEOUT_SEC seconds.
			const int select_result = ::select(_socket + 1, &read_set, nullptr, nullptr, &POLL_TIMEOUT_SEC);

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
			ssize_t received_size = ::recv(_socket, data, size, 0);
			// TODO: Need do receiving through a loop, because current realization can miss trailing bytes of message.
			if (received_size == SOCKET_ERROR) {
				handle_error(SocketErrorGroup::RECEIVE, WSAGetLastError());
				break;
			}

			if (received_size == 0) {
				// TODO: Handle connection close;
				break;
			}

			// TODO Вероятно, необходимо выполнить синхронизацию потоков для вызова колбэка
			// (смотреть std::mutex, https://habrahabr.ru/post/182610/).
			_is_reading = false;
			// TODO надо как-то вызывать колбэк on_complete в основном потоке
			// Типа сделать join текущего потока к основному и только потом выполнить колбэк.
			on_complete();
			return;
		}

		_is_reading = false;
	}
}

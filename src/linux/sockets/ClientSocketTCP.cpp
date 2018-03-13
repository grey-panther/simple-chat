#include "Logger.hpp"
#include "SocketAddress.hpp"
#include "ClientSocketTCP.hpp"
#include <poll.h>


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
		if (result == -1) {
			handle_error(SocketErrorGroup::CONNECT, errno, server_address);
		}
	}


	void ClientSocketTCP::send(const std::string& message)
	{
		ssize_t result = ::send(_socket, message.c_str(), message.size(), 0);
		if (result == -1) {
			handle_error(SocketErrorGroup::SEND, errno);
		}
	}


	std::string ClientSocketTCP::receive() const
	{
		static const std::size_t RECEIVED_DATA_SIZE = 100;
		std::shared_ptr<char> data(new char[RECEIVED_DATA_SIZE]);

		ssize_t received_size = recv(_socket, data.get(), RECEIVED_DATA_SIZE, 0);
		// TODO: Do receiving through a loop, because current implementation can miss trailing bytes of message.

		if (received_size == -1) {
			handle_error(SocketErrorGroup::RECEIVE, errno);
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
			Logger::channel(WARN) << "The not null pointer to data is required.";
		}

		_is_reading = true;
		_reading_thread.reset(new std::thread(std::bind(&ClientSocketTCP::do_reading, this, data, size, on_complete)));
	}


	void ClientSocketTCP::write(const char* data, const std::size_t size,
								const IClientSocketTCP::on_complete_callback_t& on_complete) const
	{
		ssize_t result = ::send(_socket, data, size, 0);
		if (result == -1) {
			handle_error(SocketErrorGroup::SEND, errno);
			return;
		}
		on_complete();
	}


	void ClientSocketTCP::do_reading(char* data, const std::size_t size,
									 const on_complete_callback_t& on_complete)
	{
		pollfd poll_data;
		poll_data.events = POLLIN;
		poll_data.fd = _socket;

		while (_is_reading) {
			// Wait until POLLIN event happens, but no longer than POLL_TIMEOUT_SEC seconds.
			static const std::size_t POLL_TIMEOUT_SEC = 10;
			const int poll_result = ::poll(&poll_data, 1, POLL_TIMEOUT_SEC);

			if (poll_result == -1) {
				handle_error(SocketErrorGroup::POLL, errno);
				break;
			}

			// If timeout has happened.
			if (poll_result == 0) {
				continue;
			}

			if (poll_data.revents & POLLIN) {
				// Do recv() immediately, because POLLIN event has happened.
				ssize_t received_size = recv(_socket, data, size, 0);
				// TODO: Do receiving through a loop, because current implementation can miss trailing bytes of message.
				if (received_size == -1) {
					handle_error(SocketErrorGroup::RECEIVE, errno);
					break;
				}
				if (received_size == 0) {
					// TODO: Handle connection close;
					break;
				}

				_is_reading = false;
				// TODO Use tasks queue to call callbacks.
				on_complete();
				return;
			}
		}

		_is_reading = false;
	}
}

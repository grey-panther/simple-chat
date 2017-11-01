#include "Logger.hpp"
#include "SocketAddress.hpp"
#include "ClientSocketTCP.hpp"
#include <memory>


namespace sockets
{
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
		// TODO: Realise receiving through a loop, because current realisation can miss trailing bytes of message.

		if (received_size == -1) {
			handle_error(SocketErrorGroup::RECEIVE, errno);
			return "";
		}

		return std::string(data.get(), static_cast<std::size_t>(received_size));
	}
}

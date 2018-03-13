#include "SocketUDP.hpp"
#include "Logger.hpp"
#include <memory>


namespace sockets
{
	void SocketUDP::send_to(const std::string& message, const ISocketAddress& address) const
	{
		ssize_t err = sendto(socket(), message.c_str(), message.size(), 0, address.inet_sockaddr(), address.inet_sockaddr_size());

		if (err == SOCKET_ERROR) {
			int last_err = WSAGetLastError();
			handle_error(SocketErrorGroup::SEND, last_err, address);
		}
	}


	std::string SocketUDP::receive() const
	{
		static const std::size_t DATA_SIZE = 100;
		std::shared_ptr<char> data(new char[DATA_SIZE]);

		ssize_t received_size = recvfrom(socket(), data.get(), DATA_SIZE, 0, 0, 0);

		if (received_size == SOCKET_ERROR) {
			handle_error(SocketErrorGroup::RECEIVE, WSAGetLastError());
		}

		return std::string(data.get(), static_cast<std::size_t>(received_size));
	}
}

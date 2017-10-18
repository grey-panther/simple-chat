#include "SocketUDP.hpp"
#include "Logger.hpp"
#include <memory>


void SocketUDP::send_to(const std::string& message, const ISocketAddress& address) const
{
	ssize_t err = sendto(_socket, message.c_str(), message.size(), 0, address.inet_sockaddr(), address.inet_sockaddr_size());

	if (err == -1) {
		handle_error(SocketErrorGroup::SEND, errno, address);
	}
}


std::string SocketUDP::receive() const
{
	static const std::size_t RECEIVED_DATA_SIZE = 100;
	std::shared_ptr<char> data(new char[RECEIVED_DATA_SIZE]);

	ssize_t received_size = recvfrom(_socket, data.get(), RECEIVED_DATA_SIZE, 0, 0, 0);

	if (received_size == -1) {
		handle_error(SocketErrorGroup::RECEIVE, errno);
		return "";
	}

	return std::string(data.get(), static_cast<std::size_t>(received_size));
}

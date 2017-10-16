#include "Socket.hpp"
#include "Logger.hpp"
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <memory>


Socket::Socket()
{
	_socket = socket(PF_INET, SOCK_DGRAM, 0);

	if (_socket == -1) {
		handle_error(SocketErrorGroup::OPEN, errno);
	}
}


Socket::~Socket()
{
	int close_result = close(_socket);

	if (close_result == -1) {
		handle_error(SocketErrorGroup::CLOSE, errno);
	}
}

void Socket::set_address(const ISocketAddress& address)
{
	int error = bind(_socket, address.inet_sockaddr(), address.inet_sockaddr_size());

	if (error == -1) {
		handle_error(SocketErrorGroup::SET_ADDR, errno);
	}
}


void Socket::send_to(const std::string& message, const ISocketAddress& address) const
{
	ssize_t err = sendto(_socket, message.c_str(), message.size(), 0, address.inet_sockaddr(), address.inet_sockaddr_size());

	if (err == -1) {
		handle_error(SocketErrorGroup::SEND, errno, address);
	}
}


std::string Socket::receive() const
{
	static const std::size_t DATA_SIZE = 100;
	std::shared_ptr<char> data(new char[DATA_SIZE]);

	ssize_t err = recvfrom(_socket, data.get(), DATA_SIZE, 0, 0, 0);

	if (err == -1) {
		handle_error(SocketErrorGroup::RECEIVE, errno);
	}

	return std::string(data.get(), DATA_SIZE);
}


void Socket::handle_error(SocketErrorGroup group, int error_code, std::string info)
{
	std::string action;
	switch (group) {
		case (SocketErrorGroup::OPEN):
			action = "open socket";
			break;
		case (SocketErrorGroup::SET_ADDR):
			action = "set socket address";
			break;
		case (SocketErrorGroup::SEND):
			action = "send message to " + info;
			break;
		case (SocketErrorGroup::RECEIVE):
			action = "receive message";
			break;
		case (SocketErrorGroup::CLOSE):
			action = "close socket";
			break;
		default:
			action = "do unknown action";
	}
	Logger::channel(ERR) << "Couldn't " << action << '.' << std::endl
						 << "\tError code: " << error_code << std::endl
						 << "\tDescription: " << strerror(error_code);
}


#include "Socket.hpp"
#include "Logger.hpp"
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>


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


#include "SocketBase.hpp"
#include "Logger.hpp"
#include <cstring>
#include <unistd.h>
#include <cerrno>


namespace sockets
{
	SocketBase::SocketBase(__socket_type socket_type)
	{
		_socket = socket(PF_INET, socket_type, 0);

		if (_socket == -1) {
			handle_error(SocketErrorGroup::OPEN, errno);
		}
	}


	SocketBase::~SocketBase()
	{
		int close_result = close(_socket);

		if (close_result == -1) {
			handle_error(SocketErrorGroup::CLOSE, errno);
		}
	}


	void SocketBase::set_address_impl(const ISocketAddress& address)
	{
		int error = bind(_socket, address.inet_sockaddr(), address.inet_sockaddr_size());

		if (error == -1) {
			handle_error(SocketErrorGroup::SET_ADDR, errno);
		}
	}


	void SocketBase::handle_error(SocketErrorGroup group, int error_code, std::string info)
	{
		std::string action;
		switch (group) {
			case (SocketErrorGroup::OPEN):
				action = "open sockets";
				break;
			case (SocketErrorGroup::SET_ADDR):
				action = "set sockets address";
				break;
			case (SocketErrorGroup::LISTEN):
				action = "start listening";
				break;
			case (SocketErrorGroup::CONNECT):
				action = "connect to " + info;
				break;
			case (SocketErrorGroup::POLL):
				action = "poll sockets";
				break;
			case (SocketErrorGroup::ACCEPT):
				action = "accept connection";
				break;
			case (SocketErrorGroup::SEND):
				action = "send message to " + info;
				break;
			case (SocketErrorGroup::RECEIVE):
				action = "receive message";
				break;
			case (SocketErrorGroup::CLOSE):
				action = "close sockets";
				break;
			default:
				action = "do unknown action";
		}
		Logger::channel(ERR) << "Couldn't " << action << '.' << std::endl
							 << "\tError code: " << error_code << std::endl
							 << "\tDescription: " << strerror(error_code);
	}
}

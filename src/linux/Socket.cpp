#include "Socket.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <Logger.hpp>
#include <errors/SocketError.hpp>


Socket::Socket()
{
	_socket = socket(PF_INET, SOCK_STREAM, 0);

	if (_socket < 0) {
		Logger::channel(ERR) << "Couldn't create socket. Error code: " << errno;
	}
}


Socket::~Socket()
{
	int close_result = close(_socket);

	if (close_result < 0) {
		Logger::channel(ERR) << "Couldn't destroy socket. Error code: " << errno;
	}
}


void Socket::bind()
{
	// TODO: implement
}


// TODO Решить, буду ли использовать нижележащий код
enum class SocketErrorGroup {
	OPEN,
	CLOSE
};


void handle_socket_error(SocketErrorGroup error_group, int error_id) {
	std::string message;

	switch (error_group) {
		case SocketErrorGroup::OPEN:
			switch (error_id) {
				case EACCES:
					message = "EACCES";
					break;
				case EAFNOSUPPORT:
					message = "EAFNOSUPPORT";
					break;
				case EINVAL:
					message = "EINVAL";
					break;
				case EMFILE:
					message = "EMFILE";
					break;
				case ENFILE:
					message = "ENFILE";
					break;
				case ENOBUFS:
					message = "ENOBUFS";
					break;
				case ENOMEM:
					message = "ENOMEM";
					break;
				case EPROTONOSUPPORT:
					message = "EPROTONOSUPPORT";
					break;
				default:
					message = "unknown error " + error_id;
					break;
			}
			break;

		case SocketErrorGroup::CLOSE:
			switch (error_id) {
				case EBADF:
					message = "EBADF";
					break;
				case EINTR:
					message = "EINTR";
					break;
				case EIO:
					message = "EIO";
					break;
				default:
					message = "unknown error " + error_id;
					break;
			}
			break;

		default:
			message = "Unknown error group";
			break;
	}

	Logger::channel(ERROR) << "Socket error: " << message;
}

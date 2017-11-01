#include <Logger.hpp>
#include "Socket.hpp"


std::size_t Socket::_socketsCount = 0;

Socket::Socket()
{
	if (_socketsCount == 0) {
		initializeWindowsSockets();
	}
	++_socketsCount;

	// Create sockets
	_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (_socket == INVALID_SOCKET) {
		Logger::channel(ERR) << "Couldn't create sockets. Error code: " << WSAGetLastError();
	}
}


Socket::~Socket()
{
	// Close sockets
	if (FAILED(closesocket(_socket))) {
		Logger::channel(ERR) << "Couldn't destroy sockets. Error code: " << WSAGetLastError();
	}

	--_socketsCount;
	if (_socketsCount < 1) {
		releaseWindowsSockets();
	}
}


void Socket::bind()
{
	Logger::channel(INFO) << "Do bind...";
	// TODO: implement
}


void Socket::initializeWindowsSockets()
{
	WSADATA data;					// Socket Data
	WORD version = MAKEWORD(2, 0);	// Winsock interface version

	if (FAILED(WSAStartup(version, &data))) {
		Logger::channel(ERR) << "Windows sockets initialization error.";
	}
}


void Socket::releaseWindowsSockets()
{
	if (FAILED(WSACleanup())) {
		Logger::channel(ERR) << "Windows sockets releasing error. Error code: " << WSAGetLastError();
	}
}

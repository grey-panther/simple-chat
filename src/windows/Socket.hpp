#ifndef SIMPLE_CHAT_SOCKET_HPP
#define SIMPLE_CHAT_SOCKET_HPP

#include <ISocket.hpp>
#include <winsock.h>


class Socket : public ISocket
{
	static std::size_t _socketsCount;
	SOCKET _socket;								///< Socket descriptor

public:
	Socket();

	~Socket() override;

	void bind() override;

private:
	static void initializeWindowsSockets();

	static void releaseWindowsSockets();
};


#endif

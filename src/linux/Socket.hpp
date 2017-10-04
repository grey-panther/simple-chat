#ifndef SIMPLE_CHAT_SOCKET_HPP
#define SIMPLE_CHAT_SOCKET_HPP

#include <ISocket.hpp>


class Socket : public ISocket
{
	int _socket;	///< Socket descriptor

public:
	Socket();

	~Socket() override;

	void bind() override;
};


#endif

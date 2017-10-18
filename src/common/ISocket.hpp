#ifndef SIMPLE_CHAT_ISOCKET_HPP
#define SIMPLE_CHAT_ISOCKET_HPP


#include "ISocketAddress.hpp"


class ISocket
{
public:
	ISocket() {};
	ISocket(ISocket&) = delete;
	ISocket& operator=(ISocket&) = delete;

	virtual ~ISocket() {};

	virtual void set_address(const ISocketAddress& address) = 0;
};

#endif

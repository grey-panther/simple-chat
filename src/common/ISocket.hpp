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

	virtual void send_to(const std::string& message, const ISocketAddress& address) const = 0;
};

#endif

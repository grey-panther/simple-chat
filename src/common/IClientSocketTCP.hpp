#ifndef SIMPLE_CHAT_ICLIENTSOCKETTCP_HPP
#define SIMPLE_CHAT_ICLIENTSOCKETTCP_HPP

#include "ISocket.hpp"
#include <string>


class IClientSocketTCP : public ISocket
{
public:
	virtual void connect(const ISocketAddress& server_address) = 0;

	virtual void send(const std::string& message) = 0;

	virtual std::string receive() const = 0;
};

#endif

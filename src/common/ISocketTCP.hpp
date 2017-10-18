#ifndef SIMPLE_CHAT_ISOCKETTCP_HPP
#define SIMPLE_CHAT_ISOCKETTCP_HPP

#include "ISocket.hpp"
#include <functional>
#include <memory>
#include <string>


class ISocketTCP : public ISocket
{
public:
	typedef std::function<void (std::shared_ptr<ISocketTCP>)> on_accept_connection_callback_t;

public:
	virtual void listen(on_accept_connection_callback_t on_accept_connection) = 0;

	virtual void connect(const ISocketAddress& server_address) = 0;

	virtual void send(const std::string& message) = 0;

	virtual std::string receive() const = 0;
};

#endif

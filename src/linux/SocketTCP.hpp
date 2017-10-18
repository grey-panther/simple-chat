#ifndef SIMPLE_CHAT_SOCKETTCP_HPP
#define SIMPLE_CHAT_SOCKETTCP_HPP

#include "ISocketTCP.hpp"
#include "SocketBase.hpp"


class SocketTCP : public ISocketTCP, protected SocketBase
{
public:
	SocketTCP() : SocketBase(SOCK_STREAM)
	{}

	~SocketTCP() override
	{}

	void set_address(const ISocketAddress& address) override
	{ return set_address_impl(address); }


	void listen(on_accept_connection_callback_t on_accept_connection) override;


	void connect(const ISocketAddress& server_address) override;


	void send(const std::string& message) override;


	std::string receive() const override;
};


#endif

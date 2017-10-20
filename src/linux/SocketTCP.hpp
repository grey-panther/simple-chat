#ifndef SIMPLE_CHAT_SOCKETTCP_HPP
#define SIMPLE_CHAT_SOCKETTCP_HPP

#include <thread>
#include "ISocketTCP.hpp"
#include "SocketBase.hpp"


class SocketTCP : public ISocketTCP, protected SocketBase
{
	std::shared_ptr<std::thread> listening_thread;

public:
	SocketTCP() : SocketBase(SOCK_STREAM)
	{}

	~SocketTCP() override
	{}

	void set_address(const ISocketAddress& address) override
	{ return set_address_impl(address); }


	void listen(const on_accept_connection_callback_t& on_accept_connection) override;


	void connect(const ISocketAddress& server_address) override;


	void send(const std::string& message) override;


	std::string receive() const override;

private:
	SocketTCP(int socket) : SocketBase(socket)
	{}

	void do_listening(const on_accept_connection_callback_t& on_accept_connection) const;
};


#endif

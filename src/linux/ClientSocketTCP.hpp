#ifndef SIMPLE_CHAT_CLIENTSOCKETTCP_HPP
#define SIMPLE_CHAT_CLIENTSOCKETTCP_HPP

#include "IClientSocketTCP.hpp"
#include "SocketBase.hpp"


class ClientSocketTCP : public IClientSocketTCP, protected SocketBase
{

public:
	ClientSocketTCP() : SocketBase(SOCK_STREAM)
	{}


	ClientSocketTCP(int socket) : SocketBase(socket)
	{}


	~ClientSocketTCP() override
	{}


	void set_address(const ISocketAddress& address) override
	{ return set_address_impl(address); }


	void connect(const ISocketAddress& server_address) override;


	void send(const std::string& message) override;


	std::string receive() const override;
};


#endif

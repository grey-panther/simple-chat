#ifndef SIMPLE_CHAT_SOCKETUDP_HPP
#define SIMPLE_CHAT_SOCKETUDP_HPP

#include "ISocketUDP.hpp"
#include "SocketBase.hpp"


class SocketUDP : public ISocketUDP, protected SocketBase
{
public:
	SocketUDP() : SocketBase(SOCK_DGRAM)
	{}

	~SocketUDP() override
	{}

	void set_address(const ISocketAddress& address) override
	{ return set_address_impl(address); }

	void send_to(const std::string& message, const ISocketAddress& address) const override;

	std::string receive() const override;
};


#endif

#ifndef SIMPLE_CHAT_SOCKETADDRESS_HPP
#define SIMPLE_CHAT_SOCKETADDRESS_HPP


#include "ISocketAddress.hpp"
#include <netinet/in.h>


class SocketAddress : public ISocketAddress
{
	sockaddr_in _address;

public:
	SocketAddress(const std::string& ip = "127.0.0.1", unsigned short port = 4242);


	SocketAddress(const sockaddr_in& address);


	~SocketAddress() override
	{};


	std::string ip() const override;


	void set_ip(const std::string& ip) override;


	unsigned short port() const override;


	void set_port(unsigned short port) override;


	const sockaddr* inet_sockaddr() const override;

	const unsigned int inet_sockaddr_size() const override;
};


#endif

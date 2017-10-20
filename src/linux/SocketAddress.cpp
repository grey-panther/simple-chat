#include <arpa/inet.h>
#include <Logger.hpp>
#include "SocketAddress.hpp"


SocketAddress::SocketAddress(const std::string& ip, unsigned short port)
		: _address(sockaddr_in())
{
	_address.sin_family = AF_INET;
	_address.sin_port = htons(port);
	set_ip(ip);
}


SocketAddress::SocketAddress(const sockaddr_in& address)
		: _address(address)
{
}


std::string SocketAddress::ip() const
{
	return inet_ntoa(_address.sin_addr);
}


void SocketAddress::set_ip(const std::string& ip)
{
	_address.sin_addr.s_addr = inet_addr(ip.c_str());
	if (_address.sin_addr.s_addr == INADDR_NONE) {
		Logger::channel(ERR) << "Cannot set ip-address: " << ip;
	}
}


unsigned short SocketAddress::port() const
{
	return ntohs(_address.sin_port);
}


void SocketAddress::set_port(unsigned short port)
{
	_address.sin_port = htons(port);
}


const sockaddr* SocketAddress::inet_sockaddr() const
{
	return reinterpret_cast<const sockaddr*>(&_address);
}


const unsigned int SocketAddress::inet_sockaddr_size() const
{
	return sizeof(_address);
}

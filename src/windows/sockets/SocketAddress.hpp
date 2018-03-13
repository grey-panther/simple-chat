#ifndef SIMPLE_CHAT_SOCKETADDRESS_HPP
#define SIMPLE_CHAT_SOCKETADDRESS_HPP

#include "sockets/ISocketAddress.hpp"
#include <winsock2.h>


namespace sockets
{
	class SocketAddress : public ISocketAddress
	{
		sockaddr_in _address;

	public:
		explicit SocketAddress(const std::string& ip = "127.0.0.1", unsigned short port = 4242);

		explicit SocketAddress(const sockaddr_in& address);

		~SocketAddress() override = default;

		std::string ip() const override;

		void set_ip(const std::string& ip) override;

		unsigned short port() const override;

		void set_port(unsigned short port) override;

		const sockaddr* inet_sockaddr() const override;

		const unsigned int inet_sockaddr_size() const override;
	};
}

#endif

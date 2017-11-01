#ifndef SIMPLE_CHAT_ISOCKETADDRESS_HPP
#define SIMPLE_CHAT_ISOCKETADDRESS_HPP

#include <string>
#include <ostream>


struct sockaddr;


namespace sockets
{
	class ISocketAddress
	{
	public:
		virtual ~ISocketAddress()
		{}


		virtual std::string ip() const = 0;

		virtual void set_ip(const std::string& ip) = 0;

		virtual unsigned short port() const = 0;

		virtual void set_port(unsigned short port) = 0;

		virtual const sockaddr* inet_sockaddr() const = 0;

		virtual const unsigned int inet_sockaddr_size() const = 0;

		operator std::string() const;
	};

	std::ostream& operator<<(std::ostream& out, const ISocketAddress& address);
}
#endif

#ifndef SIMPLE_CHAT_ISOCKET_HPP
#define SIMPLE_CHAT_ISOCKET_HPP

#include "ISocketAddress.hpp"


namespace sockets
{
	class ISocket
	{
	public:
		ISocket() = default;

		virtual ~ISocket() = default;

		ISocket(ISocket&) = delete;

		ISocket& operator=(ISocket&) = delete;

		/**
		 * @brief Set socket address.
		 */
		virtual void set_address(const ISocketAddress& address) = 0;
	};
}

#endif

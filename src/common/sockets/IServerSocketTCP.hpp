#ifndef SIMPLE_CHAT_ISERVERSOCKETTCP_HPP
#define SIMPLE_CHAT_ISERVERSOCKETTCP_HPP

#include "sockets/forwards.hpp"
#include "sockets/ISocket.hpp"


namespace sockets
{
	class IServerSocketTCP : public ISocket
	{
	public:
		typedef std::function<void(sockets::IClientSocketTCPSPtr)> on_accept_connection_callback_t;

	public:
		~IServerSocketTCP() override = default;

		/**
		 * @brief Start listening connections.
		 * @param on_accept_connection Callback that is called when a connection is accept.
		 */
		virtual void listen(const on_accept_connection_callback_t& on_accept_connection) = 0;

		// TODO Maybe it has sense to make method stop_listening();
	};
}

#endif

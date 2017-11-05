#ifndef SIMPLE_CHAT_SERVERSOCKETTCP_HPP
#define SIMPLE_CHAT_SERVERSOCKETTCP_HPP

#include "sockets/IServerSocketTCP.hpp"
#include "sockets/SocketBase.hpp"
#include <mingw.thread.h> // Is used instead of <thread> that is not supported with default mingw.


namespace sockets
{
	class ServerSocketTCP : protected SocketBase, public IServerSocketTCP
	{
		std::shared_ptr<std::thread> _listening_thread;
		bool _is_listening;

	public:
		ServerSocketTCP()
				: SocketBase(SOCK_STREAM)
				, _is_listening(false)
		{
		}


		~ServerSocketTCP() override;


		void set_address(const ISocketAddress& address) override
		{ return set_address_impl(address); }


		void listen(const on_accept_connection_callback_t& on_accept_connection) override;

	private:
		void do_listening(const on_accept_connection_callback_t& on_accept_connection);
	};
}


#endif

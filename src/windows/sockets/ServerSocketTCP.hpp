#ifndef SIMPLE_CHAT_SERVERSOCKETTCP_HPP
#define SIMPLE_CHAT_SERVERSOCKETTCP_HPP

#include "sockets/IServerSocketTCP.hpp"
#include "sockets/SocketBase.hpp"
#include <mingw.thread.h> // Is used instead of <thread> that is not supported with default mingw.
#include <atomic>


namespace sockets
{
	// TODO enable shared в базовый класс
	class ServerSocketTCP : protected SocketBase, public IServerSocketTCP, public std::enable_shared_from_this<ServerSocketTCP>
	{
		std::shared_ptr<std::thread> _listen_thread;
		std::atomic<bool> _is_listening;

	public:
		ServerSocketTCP(TasksProcessor& tasks_processor)
				: SocketBase(SOCK_STREAM)
				, IServerSocketTCP(tasks_processor)
				, _is_listening(false)
		{
		}


		~ServerSocketTCP() override;


		void set_address(const ISocketAddress& address) override
		{ return set_address_impl(address); }


		void listen(const on_accept_connection_callback_t& on_accept_connection) override;

	private:
		void do_listen(const on_accept_connection_callback_t& on_accept_connection);
	};
}


#endif

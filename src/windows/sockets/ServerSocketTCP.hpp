#ifndef SIMPLE_CHAT_SERVERSOCKETTCP_HPP
#define SIMPLE_CHAT_SERVERSOCKETTCP_HPP

#include "sockets/IServerSocketTCP.hpp"
#include "sockets/SocketBase.hpp"
#include <mingw.thread.h> // Is used instead of <thread> that is not supported with default mingw.
#include <atomic>


namespace sockets
{
	class ServerSocketTCP : protected SocketBase, public IServerSocketTCP
	{
		std::shared_ptr<std::thread> _listening_thread;
		std::atomic<bool> _is_listening;

	public:
		explicit ServerSocketTCP(const ITasksQueueSPtr& tasks_queue);

		~ServerSocketTCP() override;

		void set_address(const ISocketAddress& address) override
		{ return SocketBase::set_address(address); }

		void listen(const on_accept_connection_callback_t& on_accept_connection) override;

	private:
		/**
		 * @brief Listen connections in loop.
		 * @details If a new connection is accepted,
		 * a new client socket will be created and transferred to @a on_accept_connection callback.
		 * @param on_accept_connection Callback that is called when a connection is accept.
		 */
		void do_listen(const on_accept_connection_callback_t& on_accept_connection);
	};
}


#endif

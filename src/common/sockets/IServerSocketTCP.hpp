#ifndef SIMPLE_CHAT_ISERVERSOCKETTCP_HPP
#define SIMPLE_CHAT_ISERVERSOCKETTCP_HPP

#include "sockets/ISocket.hpp"
#include "sockets/IClientSocketTCP.hpp"
#include <functional>
#include <memory>


namespace sockets
{
	class IServerSocketTCP : public ISocket
	{
	public:
		typedef std::function<void(std::shared_ptr<IClientSocketTCP>)> on_accept_connection_callback_t;

	public:
		IServerSocketTCP(TasksProcessor& tasks_processor) : ISocket(tasks_processor)
		{}

		virtual void listen(const on_accept_connection_callback_t& on_accept_connection) = 0;
	};
}

#endif

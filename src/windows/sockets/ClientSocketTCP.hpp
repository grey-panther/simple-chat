#ifndef SIMPLE_CHAT_CLIENTSOCKETTCP_HPP
#define SIMPLE_CHAT_CLIENTSOCKETTCP_HPP

#include "sockets/IClientSocketTCP.hpp"
#include "sockets/SocketBase.hpp"
#include <mingw.thread.h> // Is used instead of <thread> that is not supported with default mingw.
#include <atomic>


namespace sockets
{
	class ClientSocketTCP : public IClientSocketTCP, protected SocketBase
	{
		std::shared_ptr<std::thread> _reading_thread;
		std::atomic<bool> _is_reading;

	public:
		explicit ClientSocketTCP(const ITasksQueueSPtr& tasks_queue);

		ClientSocketTCP(const ITasksQueueSPtr& tasks_queue, SOCKET socket);

		~ClientSocketTCP() override;

		void set_address(const ISocketAddress& address) override
		{ return SocketBase::set_address(address); }

		void connect(const ISocketAddress& server_address, const on_complete_callback_t& on_complete) override;

		void read(char* data, std::size_t size, const on_complete_callback_t& on_complete) override;

		void write(const char* data, std::size_t size, const on_complete_callback_t& on_complete) const override;

	private:
		/**
		 * @brief Read data from socket in loop.
		 */
		void do_read(char* data, std::size_t data_size,
					 const IClientSocketTCP::on_complete_callback_t& on_complete);
	};
}


#endif

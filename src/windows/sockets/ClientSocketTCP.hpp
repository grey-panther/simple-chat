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
		std::shared_ptr<std::thread> _read_thread;
		std::atomic<bool> _is_reading;
//		char* _read_data;
//		std::size_t _read_data_size;
//		on_complete_callback_t _on_read_complete;

	public:
		ClientSocketTCP(TasksProcessor& tasks_processor)
				: SocketBase(SOCK_STREAM)
				, IClientSocketTCP(tasks_processor)
				, _is_reading(false)
		{}


		ClientSocketTCP(TasksProcessor& tasks_processor, SOCKET socket)
				: SocketBase(socket)
				, IClientSocketTCP(tasks_processor)
				, _is_reading(false)
		{}


		~ClientSocketTCP() override;


		void set_address(const ISocketAddress& address) override
		{ return set_address_impl(address); }


		void connect(const ISocketAddress& server_address, const on_complete_callback_t& on_complete) override;


		void send(const std::string& message) override {};


		std::string receive() const override {};


		void read(char* data, const std::size_t size, const on_complete_callback_t& on_complete) override;


		void write(const char* data, const std::size_t size, const on_complete_callback_t& on_complete) const override;

	private:
		void do_read(char* data, const std::size_t data_size,
					 const IClientSocketTCP::on_complete_callback_t& on_complete);
	};
}


#endif

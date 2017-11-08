#ifndef SIMPLE_CHAT_CLIENTSOCKETTCP_HPP
#define SIMPLE_CHAT_CLIENTSOCKETTCP_HPP

#include "sockets/IClientSocketTCP.hpp"
#include "SocketBase.hpp"
#include <thread>


namespace sockets
{
	class ClientSocketTCP : public IClientSocketTCP, protected SocketBase
	{
		std::shared_ptr<std::thread> _reading_thread;
		bool _is_reading;

	public:
		ClientSocketTCP()
				: SocketBase(SOCK_STREAM)
				, _is_reading(false)
		{}


		ClientSocketTCP(int socket)
				: SocketBase(socket)
				, _is_reading(false)
		{}


		~ClientSocketTCP() override;


		void set_address(const ISocketAddress& address) override
		{ return set_address_impl(address); }


		void connect(const ISocketAddress& server_address) override;


		void send(const std::string& message) override;


		std::string receive() const override;


		void read(char* data, const std::size_t size, const on_complete_callback_t& on_complete) override;


		void write(const char* data, const std::size_t size, const on_complete_callback_t& on_complete) const override;

	private:
		void do_reading(char* data, const std::size_t size, const on_complete_callback_t& on_complete);
	};
}


#endif

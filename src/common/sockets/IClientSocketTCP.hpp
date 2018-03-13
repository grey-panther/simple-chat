#ifndef SIMPLE_CHAT_ICLIENTSOCKETTCP_HPP
#define SIMPLE_CHAT_ICLIENTSOCKETTCP_HPP

#include "ISocket.hpp"
#include <functional>


namespace sockets
{
	class IClientSocketTCP : public ISocket
	{
	public:
		typedef std::function<void(const bool is_connection_closed)> on_complete_callback_t;

	public:
		~IClientSocketTCP() override = default;

		/**
		 * @brief Connect to a server with address @a server_address.
		 * @details When connection is complete, the callback @a on_complete is called.
		 */
		virtual void connect(const ISocketAddress& server_address, const on_complete_callback_t& on_complete) = 0;

		/**
		 * @brief Read data from socket (async).
		 * @param data Destination of read data.
		 * @param size Size of data to read. Should be not more than size of *data!
		 * @param on_complete Callback that is called when reading is complete.
		 */
		virtual void read(char* data, const std::size_t size, const on_complete_callback_t& on_complete) = 0;

		/**
		 * \brief Write data to socket.
		 * \param data Source data.
		 * \param size Size of data.
		 * \param on_complete Callback that is called when writing is complete.
		 */
		virtual void write(const char* data, const std::size_t size, const on_complete_callback_t& on_complete) const = 0;
	};
}

#endif

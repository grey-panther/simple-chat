#ifndef SIMPLE_CHAT_ICLIENTSOCKETTCP_HPP
#define SIMPLE_CHAT_ICLIENTSOCKETTCP_HPP

#include "ISocket.hpp"
#include <string>
#include <functional>


namespace sockets
{
	class IClientSocketTCP : public ISocket
	{
	public:
		typedef std::function<void()> on_complete_callback_t;

	public:
		virtual void connect(const ISocketAddress& server_address) = 0;

		/**
		 * \deprecated
		 */
		virtual void send(const std::string& message) = 0;

		/**
		 * \deprecated
		 */
		virtual std::string receive() const = 0;

		/**
		 * \brief Do async read from socket.
		 * \param data Destination of reading data.
		 * \param size Size of reading data.
		 * \param callback Function that is called when the operation is completed.
		 */
		virtual void read(char* data, const std::size_t size, const on_complete_callback_t& callback) = 0;

		/**
		 * \brief Do async write to socket.
		 * \param data Source of writing data.
		 * \param size Size of writing data.
		 * \param callback Function that is called when the operation is completed.
		 */
		virtual void write(const char* data, const std::size_t size, const on_complete_callback_t& callback) const = 0;
	};
}

#endif

#ifndef SIMPLE_CHAT_SOCKETBASE_HPP
#define SIMPLE_CHAT_SOCKETBASE_HPP

#include "sockets/ISocketAddress.hpp"
#include <winsock2.h>


namespace sockets
{
	typedef int socket_type_t;

	class SocketBase
	{
	protected:
		enum class SocketErrorGroup
		{
			OPEN,
			SET_ADDR,
			LISTEN,
			CONNECT,
			SELECT,
			ACCEPT,
			SEND,
			RECEIVE,
			CLOSE
		};

		SOCKET _socket;    ///< Socket descriptor

	protected:
		/**
		 * \param socket_type TCP/IP transport layer protocol type (TCP or UDP).
		 */
		SocketBase(socket_type_t socket_type);

		/**
		 * \brief Create socket by already existed socket handle.
		 * \param socket Socket handle.
		 */
		SocketBase(SOCKET socket);

		virtual ~SocketBase();

		void set_address_impl(const ISocketAddress& address);

		static void handle_error(SocketErrorGroup group, int error_code, std::string info = "");
	};
}


#endif

#ifndef SIMPLE_CHAT_SOCKETBASE_HPP
#define SIMPLE_CHAT_SOCKETBASE_HPP

#include "ISocketAddress.hpp"
#include <sys/socket.h>


class SocketBase
{
protected:
	enum class SocketErrorGroup
	{
		OPEN,
		SET_ADDR,
		LISTEN,
		CONNECT,
		POLL,
		ACCEPT,
		SEND,
		RECEIVE,
		CLOSE
	};

	int _socket;	///< Socket descriptor

protected:
	SocketBase(__socket_type socket_type);

	/**
	 * \brief Create socket by already existed socket handle.
	 * \param socket Socket handle.
	 */
	SocketBase(int socket) : _socket(socket)
	{}

	virtual ~SocketBase();

	void set_address_impl(const ISocketAddress& address);

	static void handle_error(SocketErrorGroup group, int error_code, std::string info = "");
};


#endif

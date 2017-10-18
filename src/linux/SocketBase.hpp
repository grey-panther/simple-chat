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
		SEND,
		RECEIVE,
		CLOSE
	};

	int _socket;	///< Socket descriptor

protected:
	SocketBase(__socket_type socket_type);

	virtual ~SocketBase();

	void set_address_impl(const ISocketAddress& address);

	static void handle_error(SocketErrorGroup group, int error_code, std::string info = "");
};


#endif

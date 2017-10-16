#ifndef SIMPLE_CHAT_SOCKET_HPP
#define SIMPLE_CHAT_SOCKET_HPP

#include <ISocket.hpp>


class Socket : public ISocket
{
	enum class SocketErrorGroup
	{
		OPEN,
		SET_ADDR,
		SEND,
		CLOSE
	};

	int _socket;	///< Socket descriptor

public:
	Socket();

	~Socket() override;

	void set_address(const ISocketAddress& address) override;

	void send_to(const std::string& message, const ISocketAddress& address) const override;

private:
	static void handle_error(SocketErrorGroup group, int error_code, std::string info = "");
};


#endif

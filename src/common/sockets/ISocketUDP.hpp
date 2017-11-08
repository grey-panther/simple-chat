#ifndef SIMPLE_CHAT_IUDPSOCKET_HPP
#define SIMPLE_CHAT_IUDPSOCKET_HPP

#include "ISocket.hpp"


namespace sockets
{
	class ISocketUDP : public ISocket
	{
	public:
		ISocketUDP(TasksProcessor& tasks_processor) : ISocket(tasks_processor)
		{}

		virtual void send_to(const std::string& message, const ISocketAddress& address) const = 0;

		virtual std::string receive() const = 0;
	};
}

#endif

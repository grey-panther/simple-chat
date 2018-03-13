#ifndef SIMPLE_CHAT_SOCKETUDP_HPP
#define SIMPLE_CHAT_SOCKETUDP_HPP

#include "sockets/ISocketUDP.hpp"
#include "SocketBase.hpp"


namespace sockets
{
	/**
	 * @warning The class is just a sandbox for trying UDP protocol.
	 */
	class SocketUDP : public ISocketUDP, protected SocketBase
	{
	public:
		explicit SocketUDP(const ITasksQueueSPtr& tasks_queue)
				: SocketBase(tasks_queue, SOCK_DGRAM)
		{}


		~SocketUDP() override = default;


		void set_address(const ISocketAddress& address) override
		{ return SocketBase::set_address(address); }


		void send_to(const std::string& message, const ISocketAddress& address) const override;

		std::string receive() const override;
	};
}

#endif

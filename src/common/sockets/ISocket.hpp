#ifndef SIMPLE_CHAT_ISOCKET_HPP
#define SIMPLE_CHAT_ISOCKET_HPP


#include "ISocketAddress.hpp"
#include "TasksProcessor.hpp"


namespace sockets
{
	class ISocket
	{
		TasksProcessor& _tasks_processor;

	public:
		ISocket(TasksProcessor& tasks_processor)
				:_tasks_processor(tasks_processor)
		{
			_tasks_processor.add_socket();
		};

		ISocket(ISocket&) = delete;

		ISocket& operator=(ISocket&) = delete;


		virtual ~ISocket()
		{
			_tasks_processor.remove_socket();
		};

		virtual void set_address(const ISocketAddress& address) = 0;

	protected:
		inline TasksProcessor& tasks() const
		{ return _tasks_processor; }
	};
}

#endif

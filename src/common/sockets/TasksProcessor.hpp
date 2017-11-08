#ifndef SIMPLE_CHAT_TASKSPROCESSOR_HPP
#define SIMPLE_CHAT_TASKSPROCESSOR_HPP

#include "sockets/forwards.hpp"
#include "Logger.hpp"
#include <atomic>
#include <queue>
#ifdef WIN32
#include <mingw.mutex.h>
#endif


namespace sockets
{
	class TasksProcessor
	{
		typedef std::function<void(void)> any_function_t;
		typedef std::pair<ISocketWPtr, any_function_t> task_t; // TODO remove unused ISocketWPtr
		typedef std::shared_ptr<task_t> task_sptr_t;

		std::queue<task_sptr_t> _tasks;
		std::size_t _registered_sockets_count;
		std::mutex _tasks_mutex;
		task_sptr_t _current_task;
		std::atomic<bool> _is_terminated;

	public:
		TasksProcessor()
				: _registered_sockets_count(0)
				, _is_terminated(false)
		{}

		inline void add_socket()
		{ ++_registered_sockets_count; }

		void remove_socket();

		void run();

		inline void stop()
		{ _is_terminated = true; }

		inline bool terminated()
		{ return _is_terminated; }

		void add_task(const ISocketWPtr& socket, const any_function_t& func);

	private:
		void process(const task_sptr_t task);
	};
}


#endif

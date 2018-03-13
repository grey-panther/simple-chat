#ifndef SIMPLE_CHAT_TASKSPROCESSOR_HPP
#define SIMPLE_CHAT_TASKSPROCESSOR_HPP

#include "sockets/ITasksQueue.hpp"
#include <atomic>
#include <memory>
#include <list>
#ifdef WIN32
#include <mingw.mutex.h>
#else
#include <mutex.h>
#endif


namespace sockets
{
	class TasksProcessor : public ITasksQueue
	{
		typedef any_function_t task_t;
		typedef std::shared_ptr<task_t> task_sptr_t;

		std::list<task_sptr_t> _tasks;
		std::mutex _tasks_mutex;
		task_sptr_t _current_task;
		std::atomic<bool> _is_stopped;

	public:
		TasksProcessor();

		~TasksProcessor() override;

		void run();

		inline void stop()
		{ _is_stopped = true; }

		inline bool stopped()
		{ return _is_stopped; }

		void add_task(const any_function_t& func) override;

	private:
		void process(const task_sptr_t& task);
	};
}


#endif

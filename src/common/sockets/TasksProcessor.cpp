#include "TasksProcessor.hpp"
#ifdef WIN32
#include <mingw.thread.h>
#else
#include <thread>
#endif


namespace sockets
{
	void TasksProcessor::remove_socket()
	{
		if (_registered_sockets_count < 1) {
			Logger::channel(ERR) << "Trying remove unregistered socket";
			return;
		}
		--_registered_sockets_count;
	}


	void TasksProcessor::run()
	{
		while (!_is_terminated) {
			while (_tasks.size()) {
				_tasks_mutex.lock();
				_current_task = _tasks.front();
				_tasks.pop();
				_tasks_mutex.unlock();

				process(_current_task);

				_current_task.reset();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	}


	void TasksProcessor::add_task(const ISocketWPtr& socket, const TasksProcessor::any_function_t& func)
	{
		if (_is_terminated) {
			Logger::channel(WARN) << "Attempt to add task to stopped tasks processor!";
			return;
		}

		_tasks_mutex.lock();
		_tasks.push(std::make_shared<task_t>(socket, func));
		_tasks_mutex.unlock();
	}


	void TasksProcessor::process(const TasksProcessor::task_sptr_t task)
	{
//		if (!task || task->first.expired()) {
		if (!task) {
			return;
		}

		task->second();
	}
}
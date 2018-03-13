#include "TasksProcessor.hpp"
#include "Logger.hpp"
#ifdef WIN32
#include <mingw.thread.h>
#else
#include <thread>
#endif


namespace sockets
{
	TasksProcessor::TasksProcessor()
			: _is_stopped(true)
	{}


	TasksProcessor::~TasksProcessor()
	{
		stop();
	}


	void TasksProcessor::run()
	{
		_is_stopped = false;

		while (!_is_stopped) {
			while (!_tasks.empty()) {
				_tasks_mutex.lock();
				_current_task = _tasks.front();
				_tasks.pop_front();
				_tasks_mutex.unlock();

				process(_current_task);

				_current_task.reset();
			}
			static const int THREAD_SLEEP_DURATION_MS = 16;
			std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_SLEEP_DURATION_MS));
		}
	}


	void TasksProcessor::add_task(const TasksProcessor::any_function_t& func)
	{
		_tasks_mutex.lock();
		_tasks.push_back(std::make_shared<task_t>(func));
		_tasks_mutex.unlock();
	}


	void TasksProcessor::process(const TasksProcessor::task_sptr_t& task)
	{
		if (task) {
			(*task)();
		}
	}
}
#ifndef SIMPLE_CHAT_ITASKSQUEUE_HPP
#define SIMPLE_CHAT_ITASKSQUEUE_HPP

#include <functional>

namespace sockets
{
	class ITasksQueue
	{
	protected:
		typedef std::function<void(void)> any_function_t;

	public:
		ITasksQueue() = default;

		virtual ~ITasksQueue() = default;

		ITasksQueue(const ITasksQueue&) = delete;

		ITasksQueue& operator=(const ITasksQueue&) = delete;

		/**
		 * @brief Add task to tasks queue.
		 * @param func A function that represents a task.
		 */
		virtual void add_task(const any_function_t& func) = 0;
	};
}

#endif

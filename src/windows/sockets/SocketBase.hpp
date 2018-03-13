#ifndef SIMPLE_CHAT_SOCKETBASE_HPP
#define SIMPLE_CHAT_SOCKETBASE_HPP

#include "sockets/forwards.hpp"
#include <winsock2.h>


namespace sockets
{
	typedef int socket_type_t;

	class SocketBase
	{
		typedef std::function<void(void)> any_function_t;

	private:
		ITasksQueueSPtr _tasks_queue;			///< Tasks queue for calling callbacks.
		SOCKET _socket;    						///< Socket descriptor.
		static std::size_t __sockets_count;		///< Existed sockets count.

	protected:
		enum class SocketErrorGroup
		{
			OPEN,
			SET_ADDR,
			LISTEN,
			CONNECT,
			SELECT,
			ACCEPT,
			SEND,
			RECEIVE,
			CLOSE
		};

	protected:
		/**
		 * \param socket_type TCP/IP transport layer protocol type (TCP or UDP).
		 */
		SocketBase(const ITasksQueueSPtr& tasks_queue, const socket_type_t& socket_type);

		/**
		 * \brief Create socket with already existed socket handle.
		 * \param socket Socket handle.
		 */
		SocketBase(const ITasksQueueSPtr& tasks_queue, const SOCKET& socket);

		virtual ~SocketBase();

		/**
		 * @brief Set socket address.
		 */
		void set_address(const ISocketAddress& address);

		/**
		 * @brief Execute some function through tasks queue.
		 */
		void execute_callback(const any_function_t& callback) const;

		/**
		 * @brief Get system socket descriptor.
		 */
		inline SOCKET socket() const
		{ return _socket; }

		/**
		 * @brief Get tasks queue.
		 */
		inline ITasksQueueSPtr tasks() const
		{ return _tasks_queue; }

		static void handle_error(SocketErrorGroup group, int error_code, std::string info = "");
	};
}


#endif

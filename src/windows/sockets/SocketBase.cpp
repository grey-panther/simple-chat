#include "SocketBase.hpp"
#include "Logger.hpp"
#include "sockets/ITasksQueue.hpp"
#include "sockets/SocketAddress.hpp"


namespace sockets
{
	std::size_t SocketBase::__sockets_count = 0;


	void initialize_windows_sockets()
	{
		WSADATA data;					// Socket Data.
		WORD version = MAKEWORD(2, 0);	// Winsock interface version.

		if (FAILED(WSAStartup(version, &data))) {
			Logger::channel(ERR) << "Windows sockets initialization error.";
		}
	}


	void release_windows_sockets()
	{
		if (FAILED(WSACleanup())) {
			Logger::channel(ERR) << "Windows sockets releasing error. Error code: " << WSAGetLastError();
		}
	}


	SocketBase::SocketBase(const ITasksQueueSPtr& tasks_queue, const socket_type_t& socket_type)
			: _tasks_queue(tasks_queue)
	{
		// Windows sockets should be initialized before being used.
		if (__sockets_count == 0) {
			initialize_windows_sockets();
		}
		++__sockets_count;

		// Create socket
		_socket = ::socket(PF_INET, socket_type, 0);
		if (_socket == INVALID_SOCKET) {
			handle_error(SocketErrorGroup::OPEN, WSAGetLastError());
		}
	}


	SocketBase::SocketBase(const ITasksQueueSPtr& tasks_queue, const SOCKET& socket)
			: _tasks_queue(tasks_queue)
			, _socket(socket)
	{
		// Windows sockets should be initialized before being used.
		if (__sockets_count == 0) {
			initialize_windows_sockets();
		}
		++__sockets_count;
	}


	SocketBase::~SocketBase()
	{
		// Close socket
		if (FAILED(closesocket(_socket))) {
			handle_error(SocketErrorGroup::CLOSE, WSAGetLastError());
		}

		// Windows sockets should be released, if they are not needed more.
		--__sockets_count;
		if (__sockets_count < 1) {
			release_windows_sockets();
		}
	}


	void SocketBase::set_address(const ISocketAddress& address)
	{
		int error = bind(_socket, address.inet_sockaddr(), address.inet_sockaddr_size());

		if (error == SOCKET_ERROR) {
			handle_error(SocketErrorGroup::SET_ADDR, WSAGetLastError());
		}
	}


	void SocketBase::execute_callback(const any_function_t& callback) const
	{
		_tasks_queue->add_task(callback);
	}


	void SocketBase::handle_error(SocketErrorGroup group, int error_code, std::string info)
	{
		std::string action;
		switch (group) {
			case (SocketErrorGroup::OPEN):
				action = "open sockets";
				break;
			case (SocketErrorGroup::SET_ADDR):
				action = "set sockets address";
				break;
			case (SocketErrorGroup::LISTEN):
				action = "start listening";
				break;
			case (SocketErrorGroup::CONNECT):
				action = "connect to " + info;
				break;
			case (SocketErrorGroup::SELECT):
				action = "select sockets";
				break;
			case (SocketErrorGroup::ACCEPT):
				action = "accept connection";
				break;
			case (SocketErrorGroup::SEND):
				action = "send message to " + info;
				break;
			case (SocketErrorGroup::RECEIVE):
				action = "receive message";
				break;
			case (SocketErrorGroup::CLOSE):
				action = "close sockets";
				break;
			default:
				action = "do unknown action";
		}

		char* description = nullptr;
		FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				static_cast<DWORD>(error_code),
				MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
				reinterpret_cast<LPSTR>(&description),
				0,
				nullptr
		);

		Logger::channel(ERR) << "Couldn't " << action << ".\n"
							 << "\tError code: " << error_code << "\n"
							 << "\tDescription: " << description;

		// There are recommendations to use function HeapFree(GetProcessHeap(), s) instead of LocalFree() in win10.
		LocalFree(description);
	}
}

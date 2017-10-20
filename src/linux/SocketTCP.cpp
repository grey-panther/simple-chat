#include "Logger.hpp"
#include "SocketAddress.hpp"
#include "SocketTCP.hpp"


void SocketTCP::listen(const on_accept_connection_callback_t& on_accept_connection)
{
	// TODO Перевести в состояние прослушивания и запретить все другие операции в классе.
	// Или сделать 2 класса: ServerSocketTCP и ClientSocketTCP
	static const int CONNECTIONS_QUEUE_LENGTH = 10;
	const int result = ::listen(_socket, CONNECTIONS_QUEUE_LENGTH);

	if (result == -1) {
		handle_error(SocketErrorGroup::LISTEN, errno);
	}

	listening_thread.reset(new std::thread(std::bind(&SocketTCP::do_listening, this, on_accept_connection)));
	listening_thread->detach();
}


void SocketTCP::connect(const ISocketAddress& server_address)
{
	int result = ::connect(_socket, server_address.inet_sockaddr(), server_address.inet_sockaddr_size());
	if (result == -1) {
		handle_error(SocketErrorGroup::CONNECT, errno, server_address);
	}
}


void SocketTCP::send(const std::string& message)
{
	ssize_t result = ::send(_socket, message.c_str(), message.size(), 0);

	if (result == -1) {
		handle_error(SocketErrorGroup::SEND, errno);
	}
}


std::string SocketTCP::receive() const
{
	static const std::size_t RECEIVED_DATA_SIZE = 100;
	std::shared_ptr<char> data(new char[RECEIVED_DATA_SIZE]);

	ssize_t received_size = recv(_socket, data.get(), RECEIVED_DATA_SIZE, 0);
	// TODO: Сделать получение циклом, потому как полученное таким образом сообщение мб неполным

	if (received_size == -1) {
		handle_error(SocketErrorGroup::RECEIVE, errno);
		return "";
	}

	return std::string(data.get(), static_cast<std::size_t>(received_size));
}


void SocketTCP::do_listening(const ISocketTCP::on_accept_connection_callback_t& on_accept_connection) const
{
	while(true) {
		sockaddr_in in_addr;
		socklen_t in_addr_len;
		const int socket_handle = ::accept(_socket, reinterpret_cast<sockaddr*>(&in_addr), &in_addr_len);
		if (socket_handle == -1) {
			handle_error(SocketErrorGroup::ACCEPT, errno);
			return;
		}

		// It can be useful to check accepted socket address (in_addr) here.
		if (in_addr_len != sizeof(struct sockaddr_in)) {
			Logger::channel(WARN) << "Accepted socket address is not 'sockaddr_in'!";
		}

		std::shared_ptr<ISocketTCP> accepted_socket(new SocketTCP(socket_handle));

		// TODO Заблочить необходимый код в этом методе (смотреть std::mutex, https://habrahabr.ru/post/182610/).
		on_accept_connection(accepted_socket);
	}
}

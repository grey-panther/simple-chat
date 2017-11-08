#include "ChatClient.hpp"
#include "sockets/SocketAddress.hpp"
#include "sockets/SocketFactory.hpp"
#include "sockets/ClientSocketTCP.hpp"
#include "ChatMessage.hpp"


ChatClient::ChatClient(
		const sockets::ISocketAddressSPtr& server_address,
		const on_message_received_t& on_message_received,
		const on_connection_closed_t& on_connection_closed
)
		: _server_address(server_address)
		, _on_message_received(on_message_received)
		, _on_connection_closed(on_connection_closed)
		, _is_connected(false)
{
	_socket.reset(sockets::factory::make<sockets::IClientSocketTCP>(_sockets_tasks_processor, sockets::SocketAddress()));
}


void ChatClient::run()
{
	if (!_socket || _sockets_tasks_processor.terminated()) {
		Logger::channel(WARN) << "ChatClient::run(). Cannot run client: terminated before start.";
		return;
	}

	if (!_server_address) {
		Logger::channel(ERR) << "ChatClient::run(). Cannot run client: need server address.";
		return;
	}

	_socket->connect(*_server_address, [this] (const bool is_connection_fail) {
		if (is_connection_fail) {
			Logger::channel(INFO) << "ChatClient::run()::lambda. Server is not available.";
			stop();
			return;
		}
		_is_connected = true;
		Logger::channel(INFO) << "ChatClient::run()::lambda. Socket is connected.";
		do_receive();
	});

	_sockets_tasks_processor.run();
}


void ChatClient::stop()
{
	_is_connected = false;
	_sockets_tasks_processor.stop();
	_socket.reset();
	_server_address.reset();
	_on_connection_closed();
}


void ChatClient::send(const ChatMessageSPtr& message)
{
	if (!message) {
		Logger::channel(WARN) << "ChatClient::send(). Empty message won't be sent.";
		return;
	}

	if (!_is_connected) {
		Logger::channel(WARN) << "ChatClient::send() << Cannot send message: client isn't connected.";
		return;
	}

	_socket->write(message->data(), message->length(), [this] (const bool is_connection_closed) {
		if (is_connection_closed) {
			Logger::channel(INFO) << "ChatClient::send()::lambda" << " connection is closed.";
			stop();
			return;
		}
		Logger::channel(INFO) << "ChatClient::send()::lambda" << " message is sent";
	});
}


void ChatClient::do_receive()
{
	if (!_is_connected) {
		Logger::channel(WARN) << "ChatClient::do_receive() << Cannot wait for message: client isn't connected.";
		return;
	}

	ChatMessageSPtr message(new ChatMessage);
	_socket->read(message->data(), message->length(), [this, message] (const bool is_connection_closed) {
		if (is_connection_closed) {
			Logger::channel(INFO) << "ChatClient::do_receive()::lambda" << " connection is closed.";
			stop();
			return;
		}
		Logger::channel(INFO) << "ChatClient::do_receive()::lambda" << " message is received";
		_on_message_received(message);
		do_receive();
	});
}

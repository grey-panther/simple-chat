#include "ChatParticipant.hpp"
#include "ChatMessage.hpp"
#include "ChatRoom.hpp"
#include "Logger.hpp"


ChatParticipant::ChatParticipant(const sockets::IClientSocketTCPSPtr& socket, ChatRoom& room)
		: _socket(socket)
		, _chat_room(room)
{
}


void ChatParticipant::start_session()
{
	_chat_room.join(shared_from_this());
	do_read();
}


void ChatParticipant::deliver(const ChatMessageSPtr& message)
{
	bool is_write_in_progress = !_written_messages.empty();
	_written_messages.push_back(message);

	if (!is_write_in_progress) {
		do_write();
	}
}


void ChatParticipant::do_read()
{
	_read_message.reset(new ChatMessage);

	ChatParticipantWPtr this_w(shared_from_this());

	_socket->read(_read_message->data(), ChatMessage::length(), [this_w] (const bool is_connection_closed) {
		if (this_w.expired()) return;
		ChatParticipantSPtr this_s = this_w.lock();

		if (is_connection_closed) {
			this_s->_chat_room.leave(this_s);
		} else {
			this_s->_chat_room.send_to_all(this_s->_read_message, this_s);
			this_s->do_read();
		}
	});
}


void ChatParticipant::do_write()
{
	ChatParticipantSPtr this_s(shared_from_this());

	_socket->write(_written_messages.front()->data(), ChatMessage::length(), [this_s] (const bool /*is_closed*/) {
		this_s->_written_messages.pop_front();

		if (!this_s->_written_messages.empty()) {
			this_s->do_write();
		}
	});
}
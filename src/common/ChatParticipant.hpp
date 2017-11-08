#ifndef SIMPLE_CHAT_CHATPARTICIPANT_HPP
#define SIMPLE_CHAT_CHATPARTICIPANT_HPP

#include "forwards.hpp"
#include "sockets/forwards.hpp"
#include "sockets/IClientSocketTCP.hpp"
#include <functional>
#include <queue>


class ChatParticipant : public std::enable_shared_from_this<ChatParticipant>
{
private:
	typedef std::deque<ChatMessageSPtr> messages_t;

private:
	sockets::IClientSocketTCPSPtr _socket;
	ChatRoom& _chat_room;
	messages_t _written_messages;
	ChatMessageSPtr _read_message;

public:
	ChatParticipant(const sockets::IClientSocketTCPSPtr& socket, ChatRoom& room);

	void start_session();

	void deliver(const ChatMessageSPtr& message);

private:
	void do_read();

	void do_write();
};

#endif

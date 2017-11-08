#ifndef SIMPLE_CHAT_CHATROOM_HPP
#define SIMPLE_CHAT_CHATROOM_HPP

#include "forwards.hpp"
#include "sockets/forwards.hpp"
#include <mingw.mutex.h> // Instead of <mutex>
#include <set>


class ChatRoom
{
	static const std::size_t ROOM_SIZE_MAX = 10;

	std::set<ChatParticipantSPtr> _participants;

public:
	void join(ChatParticipantSPtr connected_socket);

	void leave(ChatParticipantSPtr participant);

	void send_to_all(const ChatMessageSPtr& message, const ChatParticipantSPtr& sender);
};


#endif

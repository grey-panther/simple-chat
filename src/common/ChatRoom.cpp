#include "ChatRoom.hpp"
#include "ChatParticipant.hpp"
#include "ChatMessage.hpp"
#include "Logger.hpp"


void ChatRoom::join(ChatParticipantSPtr new_participant)
{
	// TODO Add condition for max chat participants.
	_participants.insert(new_participant);
	Logger::channel(INFO) << "ChatRoom::join()" << " participants count = " << _participants.size();
}


void ChatRoom::leave(ChatParticipantSPtr participant) {
	_participants.erase(participant);
	Logger::channel(INFO) << "ChatRoom::leave()" << " participants count = " << _participants.size();
}

void ChatRoom::send_to_all(const ChatMessageSPtr& message, const ChatParticipantSPtr& sender) {
	Logger::channel(INFO) << "ChatRoom::send_to_all()" << " Send message for all: " << message->data();
	Logger::channel(INFO) << "ChatRoom::send_to_all()" << " participants count = " << _participants.size();

	for (auto& participant : _participants) {
		if (participant != sender) {
			participant->deliver(message);
		}
	}
}


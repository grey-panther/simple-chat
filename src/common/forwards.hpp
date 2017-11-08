#ifndef SIMPLE_CHAT_FORWARDS_HPP
#define SIMPLE_CHAT_FORWARDS_HPP

#include <memory>


class ChatMessage;
typedef std::shared_ptr<ChatMessage> ChatMessageSPtr;

class ChatParticipant;
typedef std::shared_ptr<ChatParticipant> ChatParticipantSPtr;
typedef std::weak_ptr<ChatParticipant> ChatParticipantWPtr;

class ChatMessage;
typedef std::shared_ptr<ChatMessage> ChatMessageSPtr;

class ChatRoom;
typedef std::shared_ptr<ChatRoom> ChatRoomSPtr;
typedef std::weak_ptr<ChatRoom> ChatRoomWPtr;

#endif

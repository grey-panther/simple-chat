#ifndef SIMPLE_CHAT_CHATMESSAGE_HPP
#define SIMPLE_CHAT_CHATMESSAGE_HPP

#include <string>
#include <cstring>


class ChatMessage
{
	static const std::size_t DATA_SIZE = 100;
	char _data[DATA_SIZE];

public:
	ChatMessage(const std::string& text = "")
	{
		if (!text.empty()) {
			memcpy(_data, text.data(), std::min(text.length(), length()));
		}
	}

	char* data()
	{
		return _data;
	}

	const char* data() const
	{
		return _data;
	}

	static std::size_t length() // TODO;
	{ return DATA_SIZE; }
};


#endif

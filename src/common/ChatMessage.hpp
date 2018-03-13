#ifndef SIMPLE_CHAT_CHATMESSAGE_HPP
#define SIMPLE_CHAT_CHATMESSAGE_HPP

#include <string>
#include <array>


class ChatMessage
{
	static const std::size_t DATA_SIZE = 100;
	std::array<char, DATA_SIZE> _data;

public:
	explicit ChatMessage(const std::string& text = "")
	{
		_data.fill(0);
		if (!text.empty()) {
			text.copy(_data.data(), std::min(text.length(), _data.size()));
		}
	}

	char* data()
	{
		return _data.data();
	}

	const char* data() const
	{
		return _data.data();
	}

	static std::size_t length() // TODO Messages with dynamic length;
	{ return DATA_SIZE; }
};


#endif

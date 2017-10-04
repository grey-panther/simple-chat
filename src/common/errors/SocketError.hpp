#ifndef SIMPLE_CHAT_SOCKETERROR_HPP
#define SIMPLE_CHAT_SOCKETERROR_HPP

#include <string>
#include <exception>


class SocketError : public std::exception
{
	std::string _message;

public:
	SocketError(const std::string& message = "") noexcept
			: std::exception()
			, _message(message)
	{
	}

	const char* what() const noexcept override
	{
		return _message.c_str();
	}
};


#endif

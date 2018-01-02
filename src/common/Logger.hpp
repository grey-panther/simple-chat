#ifndef SIMPLE_CHAT_LOGGER_HPP
#define SIMPLE_CHAT_LOGGER_HPP

#include <iostream>
#include <sstream>
#ifdef WIN32
#include <mingw.mutex.h>
#else
#include <mutex>
#endif


enum LogChanel
{
	INFO,
	WARN,
	ERR,
	DEBUG
};


class Logger {
	struct Message : public std::stringstream
	{
		Message() = default;

		Message(Message&&);

		~Message() override;
	};

	static std::mutex cout_mutex;
	static bool _enabled;

public:
	static Message channel(const LogChanel& chanel);

	static void set_enabled(const bool value)
	{ _enabled = value; }

private:
	static void print(const Message& msg);

	static std::string get_description(const LogChanel& chanel);
};

#endif

#include "Logger.hpp"


Logger::Message::Message(Message&& other)
		: basic_stringstream(std::move(other))
{
}


Logger::Message::~Message()
{
	*this << std::endl;
	Logger::print(*this);
}


std::mutex Logger::__cout_mutex;
bool Logger::__enabled = true;


Logger::Message Logger::channel(const LogChanel& chanel)
{
	Message msg;
	msg << '(' << get_description(chanel) << ")\t";

	return msg;
}


void Logger::print(const Message& msg)
{
	if (!__enabled) {
		return;
	}
	__cout_mutex.lock();
	std::cout << msg.str();
	__cout_mutex.unlock();
}


std::string Logger::get_description(const LogChanel& chanel)
{
	switch (chanel) {
		case LogChanel::INFO:
			return "INFO";
		case LogChanel::WARN:
			return "WARN";
		case LogChanel::ERR:
			return "ERROR";
		case LogChanel::DEBUG:
			return "DEBUG";
	}

	return "";
}

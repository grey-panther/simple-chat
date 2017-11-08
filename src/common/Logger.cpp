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


std::mutex Logger::cout_mutex;
bool Logger::_enabled = true;


Logger::Message Logger::channel(const LogChanel& chanel)
{
	Message msg;
	msg << '(' << get_description(chanel) << ")\t";

	return msg;
}


void Logger::print(const Message& msg)
{
	if (!_enabled) {
		return;
	}
	cout_mutex.lock();
	std::cout << msg.str();
	cout_mutex.unlock();
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
	}

	return "";
}

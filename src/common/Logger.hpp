#ifndef SIMPLE_CHAT_LOGGER_HPP
#define SIMPLE_CHAT_LOGGER_HPP

#include <iostream>


enum LogChanel {
	INFO,
	WARN,
	ERR
};


class Logger {
public:
	static std::ostream& channel(LogChanel ch) {
		std::cout << std::endl;
		std::cout << '(' << getChanelMessage(ch) << ")\t";
		return std::cout;
	}

private:
	static std::string getChanelMessage(LogChanel ch) {
		switch (ch) {
			case LogChanel::INFO:
				return "INFO";
			case LogChanel::WARN:
				return "WARN";
			case LogChanel::ERR:
				return "ERROR";
		}

		return "";
	}
};

#endif

#ifndef SIMPLE_CHAT_ISOCKET_HPP
#define SIMPLE_CHAT_ISOCKET_HPP


class ISocket
{
public:
	ISocket() {};
	ISocket(ISocket&) = delete;
	ISocket& operator=(ISocket&) = delete;

	virtual void bind() = 0;
	virtual ~ISocket() {};
};

#endif

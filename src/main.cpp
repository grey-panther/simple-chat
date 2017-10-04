#include <ISocket.hpp>
#include <Logger.hpp>
#include <Socket.hpp>


int main()
{
	Logger::channel(INFO) << "Hello";

	ISocket* socket = new Socket;
	socket->bind();
	delete socket;

	return 0;
}

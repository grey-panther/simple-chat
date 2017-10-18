#include <ISocket.hpp>
#include <Logger.hpp>
#include <SocketUDP.hpp>
#include <SocketAddress.hpp>
#include <memory>


typedef std::shared_ptr<ISocket> ISocketSPtr;
typedef std::shared_ptr<ISocketUDP> ISocketUDPSPtr;


void test_udp();


int main()
{
	test_udp();

	return 0;
}


void test_udp()
{
	ISocketUDPSPtr socket(new SocketUDP);
	SocketAddress addr_1("127.0.0.1", 4242);
	socket->set_address(addr_1);

	ISocketUDPSPtr other_socket(new SocketUDP);
	SocketAddress addr_2("127.0.0.1", 4243);
	other_socket->set_address(addr_2);

	const std::string sending_msg = "foobar";
	Logger::channel(INFO) << "Sending UDP message: " << sending_msg;

	socket->send_to(sending_msg, addr_2);
	std::string received_msg = other_socket->receive();

	Logger::channel(INFO) << "Received UDP message: " << received_msg;
}

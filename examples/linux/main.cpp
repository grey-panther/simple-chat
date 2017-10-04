#include <iostream>
#include <string>

#include "socket.hpp"

using namespace std;

int main ()
{
	try
	{
		_sock_addr * sock_addr = new _sock_addr ("127.0.0.1");
        	_sock * sock = new _sock (SOCK_DGRAM);
		sock->bind (sock_addr);
		delete sock;
		delete sock_addr;
	}
	catch (_sock_exception & err)
	{
		err.print ();
	}
	catch (...)
	{
		cerr << "Unknown error" << endl;
	}
}


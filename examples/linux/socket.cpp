// Listing 2.03 nix
// file: socket.cpp
// desc: CGNP socket implemetaion

#include <cstring>
#include <iostream>

#include <unistd.h>
#include <errno.h>
	
#include "socket.hpp"
	
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// определение исключений, генерируемых нашими классами
	
_sock_exception::_sock_exception (const char * error_string)
{
	error_code = errno;					
	error_description = error_string;
}
	
void _sock_exception::print ()
{
	cerr << "*** Error in " << error_description << endl			
        << "\tCode: " << error_code << endl			// вывести сообщение об ошибке, код и описание
		<< "\tDesc: " << strerror (error_code) << endl;	// ошибки в поток ошибок
}

////////////////////////////////////////////////////////////////////////////////
// определение класса для инкапсуляции адресной структуры

_sock_addr::_sock_addr ()
{
	address = new struct sockaddr_in;
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	address->sin_port = 0;
}

_sock_addr::_sock_addr (unsigned short port)
{
	address = new struct sockaddr_in;
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = INADDR_ANY;
	address->sin_port = htons (port);
}

_sock_addr::_sock_addr (const char * ip, unsigned short port)
{
	address = new struct sockaddr_in;
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = inet_addr (ip);
	if (address->sin_addr.s_addr == INADDR_NONE)
	{
		throw _sock_exception ("_socket_address - the provided IP address seems to be invalid");
		delete address;
	}
	address->sin_port = htons (port);
}

_sock_addr::~_sock_addr ()
{
	delete address;
}

////////////////////////////////////////////////////////////////////////////////
// определение интерфейса класса _socket

_sock::_sock (int type)
{
	sd = socket (PF_INET, type, 0);			// создаем сокет
	if (sd == -1)
		throw _sock_exception ("_socket - could not create a socket");
	
}

_sock::~_sock ()
{
	int error = close (sd);				// закрываем сокет
	if (error == -1)
		throw _sock_exception ("_socket - could not close socket properly");
}

void _sock::bind (_sock_addr * addr)
{
	int error = ::bind (sd, reinterpret_cast <struct sockaddr*> (addr->get_addr()), sizeof (struct sockaddr_in));
	if (error == -1)
		throw _sock_exception ("_socket::bind - could not bind socket to a provided name");
}


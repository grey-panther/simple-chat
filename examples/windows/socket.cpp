// Listing 2.03 win
// file: socket.cpp
// desc: CGNP socket implemetaion
	
#include <cstring>
#include <iostream>
	
#include "socket.hpp"
	
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Ќиже идут функции, которые выполн€ют базовую инициализацию и очистку winsock

void _ws_init ()
{
	WORD winsock_version = MAKEWORD (2,0);		// запрашиваем winsock версии 2.0
	WSADATA winsock_data;						// сюда будут помещены данные 
												// об инициализированном интерфейсе winsock

	int error = WSAStartup (winsock_version, &winsock_data);	// непосредственно инициализци€
	if (error != 0)
		throw _sock_exception ("_winsock_intialize - could not initialize winsock");
}

void _ws_cleanup ()
{
	int error = WSACleanup ();					// очистка и деинициализаци€ winsock
	if (error == SOCKET_ERROR)
		throw _sock_exception ("_winsock_cleanup - could not perform cleaning up");
}

////////////////////////////////////////////////////////////////////////////////
// определение исключений, генерируемых нашими классами
	
_sock_exception::_sock_exception (const char * error_string)
{
	error_code = GetLastError ();			// мы используем более общую функцию
											// вместо WSAGetLastError, потому что
											// исключение может быть сгенерировано
											// в случае неудачной инициализации winsock,
											// и тогда мы не смогли бы использовать 
											// функцию WSAGetLastError()
	error_description = error_string;
}
	
void _sock_exception::print ()
{
	cerr << "*** Error in " << error_description << endl			
        << "\tCode: " << error_code << endl;			// вывести сообщение об ошибке и код
														// ошибки в небуферизуемый стандартный поток
														// вывода ошибок
														// описание ошибки можно узнать с помощью утилиты
														// ErrorLookup из средств MS Visual Studio,
														// либо при помощи функции FormatMessage
}

////////////////////////////////////////////////////////////////////////////////
// определение класса дл€ инкапсул€ции нашей адресной структуры

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
	if (sd == SOCKET_ERROR)
		throw _sock_exception ("_socket - could not create a socket");
	
}

_sock::~_sock ()
{
	int error = closesocket (sd);				// закрываем сокет
	if (error == SOCKET_ERROR)
		throw _sock_exception ("_socket - could not close socket properly");
}

void _sock::bind (_sock_addr * addr)
{
	int error = ::bind (sd, reinterpret_cast <struct sockaddr*> (addr->get_addr()), sizeof (struct sockaddr_in));
	if (error == SOCKET_ERROR)
		throw _sock_exception ("_socket::bind - could not bind socket to a provided name");
}


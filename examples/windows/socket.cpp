// Listing 2.03 win
// file: socket.cpp
// desc: CGNP socket implemetaion
	
#include <cstring>
#include <iostream>
	
#include "socket.hpp"
	
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// ���� ���� �������, ������� ��������� ������� ������������� � ������� winsock

void _ws_init ()
{
	WORD winsock_version = MAKEWORD (2,0);		// ����������� winsock ������ 2.0
	WSADATA winsock_data;						// ���� ����� �������� ������ 
												// �� ������������������ ���������� winsock

	int error = WSAStartup (winsock_version, &winsock_data);	// ��������������� ������������
	if (error != 0)
		throw _sock_exception ("_winsock_intialize - could not initialize winsock");
}

void _ws_cleanup ()
{
	int error = WSACleanup ();					// ������� � ��������������� winsock
	if (error == SOCKET_ERROR)
		throw _sock_exception ("_winsock_cleanup - could not perform cleaning up");
}

////////////////////////////////////////////////////////////////////////////////
// ����������� ����������, ������������ ������ ��������
	
_sock_exception::_sock_exception (const char * error_string)
{
	error_code = GetLastError ();			// �� ���������� ����� ����� �������
											// ������ WSAGetLastError, ������ ���
											// ���������� ����� ���� �������������
											// � ������ ��������� ������������� winsock,
											// � ����� �� �� ������ �� ������������ 
											// ������� WSAGetLastError()
	error_description = error_string;
}
	
void _sock_exception::print ()
{
	cerr << "*** Error in " << error_description << endl			
        << "\tCode: " << error_code << endl;			// ������� ��������� �� ������ � ���
														// ������ � �������������� ����������� �����
														// ������ ������
														// �������� ������ ����� ������ � ������� �������
														// ErrorLookup �� ������� MS Visual Studio,
														// ���� ��� ������ ������� FormatMessage
}

////////////////////////////////////////////////////////////////////////////////
// ����������� ������ ��� ������������ ����� �������� ���������

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
// ����������� ���������� ������ _socket

_sock::_sock (int type)
{
	sd = socket (PF_INET, type, 0);			// ������� �����
	if (sd == SOCKET_ERROR)
		throw _sock_exception ("_socket - could not create a socket");
	
}

_sock::~_sock ()
{
	int error = closesocket (sd);				// ��������� �����
	if (error == SOCKET_ERROR)
		throw _sock_exception ("_socket - could not close socket properly");
}

void _sock::bind (_sock_addr * addr)
{
	int error = ::bind (sd, reinterpret_cast <struct sockaddr*> (addr->get_addr()), sizeof (struct sockaddr_in));
	if (error == SOCKET_ERROR)
		throw _sock_exception ("_socket::bind - could not bind socket to a provided name");
}


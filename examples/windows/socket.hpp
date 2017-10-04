// Listing 2.03 win
// file: socket.hpp
// desc: CGNP socket header file

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>

#pragma comment (lib, "ws2_32.lib")
#include <winsock2.h>				
	
using namespace std;

//------------------------------------------------------------------------------
// ������������� � ��������������� winsock

void	_ws_init ();					
void	_ws_cleanup ();

////////////////////////////////////////////////////////////////////////////////
// �����, ���������� �� ��������� ������
// �������, ���� �� ��� ������ �� ������������, �� ���� ���
// ����� �������� ��������������� ���������
	
class _sock_exception
{
public:
	
	_sock_exception (const char * error_string);
	~_sock_exception () {};
	
	void 	print ();								// ������� ��������� �� ������
	int		get_code ()	{ return error_code; };		// �������� ��� ������

protected:
	
	int		error_code;
	string	error_description;
};

////////////////////////////////////////////////////////////////////////////////
// ��� �����, ������� �������� �������� ��������� ��� ������ ������

class _sock_addr
{
private:

	struct sockaddr_in * address;

public:

	_sock_addr ();
	_sock_addr (unsigned short port);
	_sock_addr (const char * ip, unsigned short port = 0);
	~_sock_addr ();

	struct sockaddr_in *	get_addr () { return address; };
	char *					get_ip ()	{ return inet_ntoa (address->sin_addr); };
	unsigned short			get_port ()	{ return ntohs (address->sin_port); };
};

////////////////////////////////////////////////////////////////////////////////
// � ��� ��� ��� ������� ����� - ��� �����
// ����� �� �� ��� �� ����� �����, � � ������� ����
// ������ ���������� ������� ������, �� �� ����� �������
// ��� ������ ������������ ����

class _sock
{
public:
	
	_sock (int type);
	~_sock ();
  	
	void	bind (_sock_addr * addr);			// �������� ������ � "����� ������"
  		
protected:
	
  	SOCKET	sd;
};

#endif // SOCKET_HPP
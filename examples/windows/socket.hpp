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
// инициализация и деинициализация winsock

void	_ws_init ();					
void	_ws_cleanup ();

////////////////////////////////////////////////////////////////////////////////
// класс, отвечающий за обработку ошибок
// конечно, пока он еще ничего не обрабатывает, но зато уже
// умеет выводить диагностические сообщения
	
class _sock_exception
{
public:
	
	_sock_exception (const char * error_string);
	~_sock_exception () {};
	
	void 	print ();								// вывести сообщение об ошибке
	int		get_code ()	{ return error_code; };		// получить код ошибки

protected:
	
	int		error_code;
	string	error_description;
};

////////////////////////////////////////////////////////////////////////////////
// это класс, который содержит адресную структуру для нашего сокета

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
// а это уже наш главный класс - сам сокет
// опять же он еще не очень силен, и с помощью него
// ничего особенного сделать нельзя, но он будет основой
// для нашего последующего кода

class _sock
{
public:
	
	_sock (int type);
	~_sock ();
  	
	void	bind (_sock_addr * addr);			// привязка сокета к "имени сокета"
  		
protected:
	
  	SOCKET	sd;
};

#endif // SOCKET_HPP
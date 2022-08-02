#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <iostream>
#include <thread>
#include <memory>
#include <system_error>

#include "asio.hpp"

using namespace asio;

typedef std::shared_ptr<ip::tcp::socket> socket_ptr;

void client_session(socket_ptr sock) {
	std::error_code err;
	while ( true) {
		char data[512];
		size_t len = sock->read_some(buffer(data), err);
		if (err == error::eof)
			return;	// 连接关闭
		if ( len > 0)
			write(*sock, buffer("ok", 2));
	}
}

int main()
{
	io_context service;

	ip::tcp::endpoint ep( ip::tcp::v4(), 2001); // listen on 2001
	ip::tcp::acceptor acc(service, ep);

	while ( true) {
		socket_ptr sock(new ip::tcp::socket(service));
		acc.accept(*sock);
		std::thread( std::bind(client_session, sock)).detach();
	}

	return 0;
}


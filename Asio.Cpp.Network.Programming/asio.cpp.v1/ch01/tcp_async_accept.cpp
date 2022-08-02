#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <iostream>
#include <thread>
#include <memory>
#include <system_error>

#include "asio.hpp"

using namespace std::placeholders;
using namespace asio;

typedef std::shared_ptr<ip::tcp::socket> socket_ptr;

io_context service;

ip::tcp::endpoint ep( ip::tcp::v4(), 2001); // listen on 2001
ip::tcp::acceptor acc(service, ep);

void handle_accept(socket_ptr sock, const std::error_code & err);

void start_accept(socket_ptr sock) {
	acc.async_accept(*sock, std::bind( handle_accept, sock, _1) );
}

void handle_accept(socket_ptr sock, const std::error_code & err) {
	if ( err) return;
	// 从这里开始, 你可以从socket读取或者写入
	socket_ptr sock_(new ip::tcp::socket(service));
	start_accept(sock_);
}

int main()
{
	socket_ptr sock(new ip::tcp::socket(service));
	start_accept(sock);
	service.run();

	return 0;
}


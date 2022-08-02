#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <system_error>
#include <iostream>
#include "asio.hpp"

using namespace asio;

void connect_handler(const std::error_code & ec) {
	// 如果ec返回成功我们就可以知道连接成功了
	if (!ec) {
		std::cout << "connect ok!" << std::endl;
	}
}

int main()
{
	io_context service;
	ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 2001);

	ip::tcp::socket sock(service);
	sock.async_connect(ep, connect_handler);

	service.run();

	return 0;
}


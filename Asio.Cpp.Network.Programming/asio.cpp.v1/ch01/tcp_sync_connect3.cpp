#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <system_error>
#include <iostream>
#include "asio.hpp"

using namespace asio;

int main()
{
	io_context service;
	ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 2001);

	ip::tcp::socket sock(service);
	std::error_code err;
	sock.connect(ep, err);
	if ( err)
		std::cout << err << std::endl;

	return 0;
}


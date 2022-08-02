#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <cstdlib>
#include <iostream>
#include "asio.hpp"

using namespace asio;

int main()
{
	io_context service;
	serial_port sp(service, "/dev/pts/26");

	serial_port::baud_rate rate(9600); 
	sp.set_option(rate);

	char data[512];
	auto len = read(sp, buffer(data, 512));

	std::cout << "read from serial_port: " << std::string(data, len) << std::endl;

	return 0;
}


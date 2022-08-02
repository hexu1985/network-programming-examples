#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <cstdlib>
#include <iostream>
#include <system_error>
#include "asio.hpp"

void signal_handler(const std::error_code & err, int signal)
{
	// 纪录日志，然后退出应用
	std::cout << "trap singal " << signal << std::endl;
}

int main()
{
	asio::io_context service;

	asio::signal_set sig(service, SIGINT, SIGTERM);
	sig.async_wait(signal_handler);

	service.run();

	return 0;
}


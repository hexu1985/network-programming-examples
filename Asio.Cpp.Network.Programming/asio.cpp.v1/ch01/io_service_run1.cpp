#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <functional>
#include <thread>
#include <chrono>
#include <system_error>
#include <iostream>
#include "asio.hpp"

using namespace std::placeholders;
using namespace asio;

void connect_handler(const std::error_code & ec, int id) {
	// 如果ec返回成功我们就可以知道连接成功了
	if (!ec) {
		std::cout << "connect ok of socket " << id << " !" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	} else {
		std::cout << "connect error of socket " << id << ": " << ec << std::endl;
	}
}

void timeout_handler(const asio::error_code&)
{
	std::cout << "handle_timeout\n";
}

int main()
{
	io_context service; // 所有socket操作都由service来处理
	ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 2001);
	ip::tcp::socket sock1(service); // all the socket operations are handled by service
	ip::tcp::socket sock2(service); 
	sock1.async_connect( ep, std::bind(connect_handler, _1, 1));
	sock2.async_connect( ep, std::bind(connect_handler, _1, 2));
	system_timer t(service, std::chrono::seconds(5));
	t.async_wait(timeout_handler);
	service.run();

	return 0;
}


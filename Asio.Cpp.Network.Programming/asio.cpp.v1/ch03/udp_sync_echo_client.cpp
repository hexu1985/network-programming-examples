#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <functional>
#include <iostream>
#include <vector>
#include <thread>
#include "asio.hpp"

using namespace std::placeholders;
using namespace asio;

io_context service;

ip::udp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);

void sync_echo(std::string msg) {
    ip::udp::socket sock(service, ip::udp::endpoint(ip::udp::v4(), 0) );
    sock.send_to(buffer(msg), ep);
    char buff[1024];
    ip::udp::endpoint sender_ep;
    int bytes = sock.receive_from(buffer(buff), sender_ep);
    std::string copy(buff, bytes);
    std::cout << "server echoed our " << msg << ": "
                << (copy == msg ? "OK" : "FAIL") << std::endl;
    sock.close();
}

int main(int argc, char* argv[]) {
    // connect several clients
    char* messages[] = { "John says hi", "so does James", "Lucy just got home", 0 };
	std::vector<std::thread> threads;
    for ( char ** message = messages; *message; ++message) {
		threads.push_back(std::thread(std::bind(sync_echo, *message)));
        std::this_thread::sleep_for( std::chrono::milliseconds(100));
    }

	for (auto &thread: threads)
		thread.join();

	return 0;
}

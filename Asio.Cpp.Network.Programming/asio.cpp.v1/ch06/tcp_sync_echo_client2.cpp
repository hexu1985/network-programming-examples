#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <functional>
#include <iostream>
#include <vector>
#include <thread>
#include <iterator>
#include "asio.hpp"

using namespace asio;

io_context service;

ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);

std::string to_string(streambuf &buf) {
	std::ostringstream out;
	out << &buf;
	return out.str();
}

void sync_echo(std::string msg) {
    msg += "\n";
    ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(buffer(msg));
	streambuf buf;	// asio::streambuf
    read_until(sock, buf, "\n");
    std::string copy = to_string(buf);
    std::cout << "server echoed our " << msg.substr(0, msg.size() - 1) << ": "
                << (copy == msg ? "OK" : "FAIL") << std::endl;
    sock.close();
}

int main(int argc, char* argv[]) {
    // connect several clients
    char* messages[] = { "John says hi", "so does James", 
                         "Lucy just got home", "Boost.Asio is Fun!", 0 };
	std::vector<std::thread> threads;
    for ( char ** message = messages; *message; ++message) {
		threads.push_back(std::thread(std::bind(sync_echo, *message)));
        std::this_thread::sleep_for( std::chrono::milliseconds(100));
    }

	for (auto &thread: threads)
		thread.join();

	return 0;
}

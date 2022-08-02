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

size_t read_complete(char * buf, const std::error_code & err, size_t bytes) {
    if ( err) return 0;
    bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);

void sync_echo(std::string msg) {
    msg += "\n";
    ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(buffer(msg));
    char buf[1024];
    int bytes = read(sock, buffer(buf), std::bind(read_complete,buf,_1,_2));
    std::string copy(buf, bytes - 1);
    msg = msg.substr(0, msg.size() - 1);
    std::cout << "server echoed our " << msg << ": "
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

#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <functional>
#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <algorithm>
#include "asio.hpp"

using namespace std::placeholders;
using namespace asio;

io_context service;

size_t read_complete(char * buff, const std::error_code & err, size_t bytes) {
    if ( err) return 0;
    bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

void handle_connections() {
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(),8001));
    char buff[1024];
    while ( true) {
        ip::tcp::socket sock(service);
        acceptor.accept(sock);
        int bytes = read(sock, buffer(buff), 
                    std::bind(read_complete,buff,_1,_2));
        std::string msg(buff, bytes);
        sock.write_some(buffer(msg));
        sock.close();
    }
}

int main(int argc, char* argv[]) {
    handle_connections();
}

#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <functional>
#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <string>
#include "asio.hpp"

using namespace std::placeholders;
using namespace asio;

io_context service;

void handle_connections() {
    char buff[1024];
    ip::udp::socket sock(service, ip::udp::endpoint(ip::udp::v4(), 8001));
    while ( true) {
        ip::udp::endpoint sender_ep;
        int bytes = sock.receive_from(buffer(buff), sender_ep);
        std::string msg(buff, bytes);
        sock.send_to(buffer(msg), sender_ep);
    }
}
int main(int argc, char* argv[]) {
    handle_connections();
}

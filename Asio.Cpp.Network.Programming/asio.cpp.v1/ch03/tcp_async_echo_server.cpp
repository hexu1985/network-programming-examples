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

#define MEM_FN(x)       std::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    std::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  std::bind(&self_type::x, shared_from_this(),y,z)

class talk_to_client : public std::enable_shared_from_this<talk_to_client> {
	talk_to_client(const talk_to_client &) = delete;
    void operator =(const talk_to_client &) = delete;	

    typedef talk_to_client self_type;
    talk_to_client() : sock_(service), started_(false) {}
public:
    typedef std::error_code error_code;
    typedef std::shared_ptr<talk_to_client> ptr;

    void start() {
        started_ = true;
        do_read();
    }
    static ptr new_() {
        ptr new_(new talk_to_client);
        return new_;
    }
    void stop() {
        if ( !started_) return;
        started_ = false;
        sock_.close();
    }
    ip::tcp::socket & sock() { return sock_;}
private:
    void on_read(const error_code & err, size_t bytes) {
        if ( !err) {
            std::string msg(read_buffer_, bytes);
            // echo message back, and then stop
            do_write(msg + "\n");
        }
        stop();
    }

    void on_write(const error_code & err, size_t bytes) {
        do_read();
    }
    void do_read() {
        async_read(sock_, buffer(read_buffer_), 
                   MEM_FN2(read_complete,_1,_2), MEM_FN2(on_read,_1,_2));
    }
    void do_write(const std::string & msg) {
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_write_some( buffer(write_buffer_, msg.size()), 
                                MEM_FN2(on_write,_1,_2));
    }
    size_t read_complete(const std::error_code & err, size_t bytes) {
        if ( err) return 0;
        bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }
private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
};

ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));

void handle_accept(talk_to_client::ptr client, const std::error_code & err) {
    client->start();
    talk_to_client::ptr new_client = talk_to_client::new_();
    acceptor.async_accept(new_client->sock(), std::bind(handle_accept,new_client,_1));
}


int main(int argc, char* argv[]) {
    talk_to_client::ptr client = talk_to_client::new_();
    acceptor.async_accept(client->sock(), std::bind(handle_accept,client,_1));
    service.run();
}


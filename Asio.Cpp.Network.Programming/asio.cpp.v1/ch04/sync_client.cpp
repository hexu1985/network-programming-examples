#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <functional>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <system_error>
#include <vector>
#include "asio.hpp"

using namespace std::placeholders;
using namespace asio;

io_context service;

/** simple connection to server:
    - logs in just with username (no password)
    - all connections are initiated by the client: client asks, server answers
    - server disconnects any client that hasn't pinged for 5 seconds

    Possible requests:
    - gets a list of all connected clients
    - ping: the server answers either with "ping ok" or "ping client_list_changed"
*/
struct talk_to_svr {
    talk_to_svr(const std::string & username) 
        : sock_(service), started_(true), username_(username) {}
    void connect(ip::tcp::endpoint ep) {
        sock_.connect(ep);
    }
    void loop() {
        // read answer to our login
        write("login " + username_ + "\n");
        read_answer();
        while ( started_) {
            write_request();
            read_answer();
            int millis = rand() % 7000;
            std::cout << username_ << " postpone ping " 
                      << millis << " ms" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(millis));
        }
    }
    std::string username() const { return username_; }
private:
    void write_request() {
        write("ping\n");
    }
    void read_answer() {
        already_read_ = 0;
        read(sock_, buffer(buff_), 
             std::bind(&talk_to_svr::read_complete, this, _1, _2));
        process_msg();
    }
    void process_msg() {
        std::string msg(buff_, already_read_);
        if ( msg.find("login ") == 0) on_login();
        else if ( msg.find("ping") == 0) on_ping(msg);
        else if ( msg.find("clients ") == 0) on_clients(msg);
        else std::cerr << "invalid msg " << msg << std::endl;
    }

    void on_login() {
        std::cout << username_ << " logged in" << std::endl;
        do_ask_clients();
    }
    void on_ping(const std::string & msg) {
        std::istringstream in(msg);
        std::string answer;
        in >> answer >> answer;
        if ( answer == "client_list_changed") 
            do_ask_clients();
    }
    void on_clients(const std::string & msg) {
        std::string clients = msg.substr(8);
        std::cout << username_ << ", new client list:" << clients;
    }
    void do_ask_clients() {
        write("ask_clients\n");
        read_answer();
    }

    void write(const std::string & msg) {
        sock_.write_some(buffer(msg));
    }
    size_t read_complete(const std::error_code & err, size_t bytes) {
        if ( err) return 0;
        already_read_ = bytes;
        bool found = std::find(buff_, buff_ + bytes, '\n') < buff_ + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }

private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    int already_read_;
    char buff_[max_msg];
    bool started_;
    std::string username_;
};

ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);
void run_client(const std::string & client_name) {
    talk_to_svr client(client_name);
    try {
        client.connect(ep);
        client.loop();
    } catch(std::system_error & err) {
        std::cout << "client terminated " << client.username() 
                  << ": " << err.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
	std::vector<std::thread> threads;
    char* names[] = { "John", "James", "Lucy", "Tracy", "Frank", "Abby", 0 };
    for ( char ** name = names; *name; ++name) {
        threads.push_back(std::thread(std::bind(run_client, *name)));
        std::this_thread::sleep_for( std::chrono::milliseconds(100));
    }

	for (auto &thread: threads)
		thread.join();

	return 0;
}


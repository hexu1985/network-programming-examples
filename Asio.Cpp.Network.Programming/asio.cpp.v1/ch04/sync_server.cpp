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

struct talk_to_client;
typedef std::shared_ptr<talk_to_client> client_ptr;
typedef std::vector<client_ptr> array;
array clients;
// thread-safe access to clients array
std::recursive_mutex cs;

void update_clients_changed() ;

/** simple connection to server:
    - logs in just with username (no password)
    - all connections are initiated by the client: client asks, server answers
    - server disconnects any client that hasn't pinged for 5 seconds

    Possible requests:
    - gets a list of all connected clients
    - ping: the server answers either with "ping ok" or "ping client_list_changed"
*/
struct talk_to_client : std::enable_shared_from_this<talk_to_client> {
    talk_to_client() 
        : sock_(service), started_(false), already_read_(0) {
		last_ping = std::chrono::system_clock::now();
    }
    std::string username() const { return username_; }

    void answer_to_client() {
        try {
            read_request();
            process_request();
        } catch ( std::system_error&) {
            stop();
        }
        if ( timed_out()) {
            stop();
            std::cout << "stopping " << username_ << " - no ping in time" << std::endl;
        }
    }
    void set_clients_changed() { clients_changed_ = true; }
    ip::tcp::socket & sock() { return sock_; }
    bool timed_out() const {
		using namespace std::chrono;
		auto now = system_clock::now();
        long long ms = duration_cast<milliseconds>(now - last_ping).count();
        return ms > 5000 ;
    }
    void stop() {
        // close client connection
        std::error_code err;
        sock_.close(err);
    }
private:
    void read_request() {
        if ( sock_.available())
            already_read_ += sock_.read_some(
                buffer(buff_ + already_read_, max_msg - already_read_));
    }
    void process_request() {
        bool found_enter = std::find(buff_, buff_ + already_read_, '\n') 
                          < buff_ + already_read_;
        if ( !found_enter)
            return; // message is not full
        // process the msg
		last_ping = std::chrono::system_clock::now();
        size_t pos = std::find(buff_, buff_ + already_read_, '\n') - buff_;
        std::string msg(buff_, pos);
        std::copy(buff_ + already_read_, buff_ + max_msg, buff_);
        already_read_ -= pos + 1;

        if ( msg.find("login ") == 0) on_login(msg);
        else if ( msg.find("ping") == 0) on_ping();
        else if ( msg.find("ask_clients") == 0) on_clients();
        else std::cerr << "invalid msg " << msg << std::endl;
    }
    
    void on_login(const std::string & msg) {
        std::istringstream in(msg);
        in >> username_ >> username_;
        std::cout << username_ << " logged in" << std::endl;
        write("login ok\n");
        update_clients_changed();
    }
    void on_ping() {
        write(clients_changed_ ? "ping client_list_changed\n" : "ping ok\n");
        clients_changed_ = false;
    }
    void on_clients() {
        std::string msg;
        { std::lock_guard<std::recursive_mutex> lk(cs);
          for( array::const_iterator b = clients.begin(), e = clients.end() ; b != e; ++b)
            msg += (*b)->username() + " ";
        }
        write("clients " + msg + "\n");
    }


    void write(const std::string & msg) {
        sock_.write_some(buffer(msg));
    }
private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    int already_read_;
    char buff_[max_msg];
    bool started_;
    std::string username_;
    bool clients_changed_;
	std::chrono::system_clock::time_point last_ping;
};

void update_clients_changed() {
	std::lock_guard<std::recursive_mutex> lk(cs);
    for( array::iterator b = clients.begin(), e = clients.end(); b != e; ++b)
        (*b)->set_clients_changed();
}



void accept_thread() {
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));
    while ( true) {
        client_ptr new_( new talk_to_client);
        acceptor.accept(new_->sock());
        
		std::lock_guard<std::recursive_mutex> lk(cs);
        clients.push_back(new_);
    }
}

void handle_clients_thread() {
    while ( true) {
        std::this_thread::sleep_for( std::chrono::milliseconds(1));
		std::lock_guard<std::recursive_mutex> lk(cs);
        for ( array::iterator b = clients.begin(), e = clients.end(); b != e; ++b) 
            (*b)->answer_to_client();
        // erase clients that timed out
        clients.erase(std::remove_if(clients.begin(), clients.end(), 
                   std::bind(&talk_to_client::timed_out,_1)), clients.end());
    }
}

int main(int argc, char* argv[]) {
	std::vector<std::thread> threads;
    threads.push_back(std::thread(accept_thread));
    threads.push_back(std::thread(handle_clients_thread));

	for (auto &thread: threads)
		thread.join();

	return 0;
}











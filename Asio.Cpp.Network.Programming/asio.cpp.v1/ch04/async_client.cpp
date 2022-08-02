#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <functional>
#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <system_error>

#include "asio.hpp"

using namespace std::placeholders;
using namespace asio;

io_context service;

#define MEM_FN(x)       std::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    std::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  std::bind(&self_type::x, shared_from_this(),y,z)

/** simple connection to server:
    - logs in just with username (no password)
    - all connections are initiated by the client: client asks, server answers
    - server disconnects any client that hasn't pinged for 5 seconds

    Possible requests:
    - gets a list of all connected clients
    - ping: the server answers either with "ping ok" or "ping client_list_changed"
*/
class talk_to_svr : public std::enable_shared_from_this<talk_to_svr> {
    typedef talk_to_svr self_type;
    talk_to_svr(const std::string & username) 
      : sock_(service), started_(true), username_(username), timer_(service) {}
    void start(ip::tcp::endpoint ep) {
        sock_.async_connect(ep, MEM_FN1(on_connect,_1));
    }

	talk_to_svr(const talk_to_svr &) = delete; 
	void operator =(const talk_to_svr &) = delete; 

public:
    typedef std::error_code error_code;
    typedef std::shared_ptr<talk_to_svr> ptr;

    static ptr start(ip::tcp::endpoint ep, const std::string & username) {
        ptr new_(new talk_to_svr(username));
        new_->start(ep);
        return new_;
    }

    void stop() {
        if ( !started_) return;
        std::cout << "stopping " << username_ << std::endl;
        started_ = false;
        sock_.close();
    }

    bool started() { return started_; }

private:
    void on_connect(const error_code & err) {
        if ( !err)      do_write("login " + username_ + "\n");
        else            stop();
    }

    void on_read(const error_code & err, size_t bytes) {
        if ( err) stop();
        if ( !started() ) return;
        // process the msg
        std::string msg(read_buffer_, bytes);
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
        if ( answer == "client_list_changed") do_ask_clients();
        else postpone_ping();
    }

    void on_clients(const std::string & msg) {
        std::string clients = msg.substr(8);
        std::cout << username_ << ", new client list:" << clients ;
        postpone_ping();
    }

    void do_ping() {
        do_write("ping\n");
    }

    void postpone_ping() {
        // note: even though the server wants a ping every 5 secs, we randomly 
        // don't ping that fast - so that the server will randomly disconnect us
        int millis = rand() % 7000;
        std::cout << username_ << " postponing ping " << millis 
                  << " millis" << std::endl;
		timer_.expires_after(std::chrono::milliseconds(millis));
        timer_.async_wait( MEM_FN(do_ping));
    }

    void do_ask_clients() {
        do_write("ask_clients\n");
    }

    void on_write(const error_code & err, size_t bytes) {
        do_read();
    }

    void do_read() {
        async_read(sock_, buffer(read_buffer_), 
                   MEM_FN2(read_complete,_1,_2), MEM_FN2(on_read,_1,_2));
    }

    void do_write(const std::string & msg) {
        if ( !started() ) return;
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
    std::string username_;
    system_timer timer_;
};

int main(int argc, char* argv[]) {
    // connect several clients
    ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);
    char* names[] = { "John", "James", "Lucy", "Tracy", "Frank", "Abby", 0 };
    for ( char ** name = names; *name; ++name) {
        talk_to_svr::start(ep, *name);
        std::this_thread::sleep_for( std::chrono::milliseconds(100));
    }

    service.run();
}















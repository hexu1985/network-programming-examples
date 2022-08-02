#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif

#include <functional>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <chrono>
#include <system_error>

#include "asio.hpp"

using namespace std::placeholders;
using namespace asio;

io_context service;

class talk_to_client;
typedef std::shared_ptr<talk_to_client> client_ptr;
typedef std::vector<client_ptr> array;
array clients;
std::recursive_mutex clients_cs;

#define MEM_FN(x)       std::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    std::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  std::bind(&self_type::x, shared_from_this(),y,z)


void update_clients_changed();

/** simple connection to server:
    - logs in just with username (no password)
    - all connections are initiated by the client: client asks, server answers
    - server disconnects any client that hasn't pinged for 5 seconds

    Possible client requests:
    - gets a list of all connected clients
    - ping: the server answers either with "ping ok" or "ping client_list_changed"
*/
class talk_to_client : public std::enable_shared_from_this<talk_to_client> {
    typedef talk_to_client self_type;
    talk_to_client() : sock_(service), started_(false), 
                       timer_(service), clients_changed_(false) {
    }

	talk_to_client(const talk_to_client &) = delete;
    void operator =(const talk_to_client &) = delete;	

public:
    typedef std::error_code error_code;
    typedef std::shared_ptr<talk_to_client> ptr;

    void start() {
        { 
			std::lock_guard<std::recursive_mutex> lk(clients_cs);
			clients.push_back( shared_from_this());
        }
		std::lock_guard<std::recursive_mutex> lk(clients_cs);
        started_ = true;
		last_ping_ = std::chrono::system_clock::now();
        // first, we wait for client to login
        do_read();
    }

    static ptr new_() {
        ptr new_(new talk_to_client);
        return new_;
    }

    void stop() {
        {
			std::lock_guard<std::recursive_mutex> lk(clients_cs);
			if ( !started_) return;
			started_ = false;
			sock_.close();
        }

        ptr self = shared_from_this();
        {
			std::lock_guard<std::recursive_mutex> lk(clients_cs);
			array::iterator it = std::find(clients.begin(), clients.end(), self);
			clients.erase(it);
        }
        update_clients_changed();
    }

    bool started() const { 
		std::lock_guard<std::recursive_mutex> lk(clients_cs);
        return started_; 
    }

    ip::tcp::socket & sock() { 
		std::lock_guard<std::recursive_mutex> lk(clients_cs);
        return sock_;
    }

    std::string username() const { 
		std::lock_guard<std::recursive_mutex> lk(clients_cs);
        return username_; 
    }

    void set_clients_changed() { 
		std::lock_guard<std::recursive_mutex> lk(clients_cs);
        clients_changed_ = true; 
    }

private:
    void on_read(const error_code & err, size_t bytes) {
        if ( err) stop();
        if ( !started() ) return;

		std::lock_guard<std::recursive_mutex> lk(clients_cs);
        // process the msg
        std::string msg(read_buffer_, bytes);
        if ( msg.find("login ") == 0) on_login(msg);
        else if ( msg.find("ping") == 0) on_ping();
        else if ( msg.find("ask_clients") == 0) on_clients();
        else std::cerr << "invalid msg " << msg << std::endl;
    }
    
    void on_login(const std::string & msg) {
		std::lock_guard<std::recursive_mutex> lk(clients_cs);
        std::istringstream in(msg);
        in >> username_ >> username_;
        std::cout << username_ << " logged in" << std::endl;
        do_write("login ok\n");
        update_clients_changed();
    }
    void on_ping() {
		std::lock_guard<std::recursive_mutex> lk(clients_cs);
        do_write(clients_changed_ ? "ping client_list_changed\n" : "ping ok\n");
        clients_changed_ = false;
    }
    void on_clients() {
        array copy;
        {
			std::lock_guard<std::recursive_mutex> lk(clients_cs);
          copy = clients;
        }
        std::string msg;
        for( array::const_iterator b = copy.begin(), e = copy.end() ; b != e; ++b)
            msg += (*b)->username() + " ";
        do_write("clients " + msg + "\n");
    }

    void do_ping() {
        do_write("ping\n");
    }
    void do_ask_clients() {
        do_write("ask_clients\n");
    }

    void on_check_ping() {
		using namespace std::chrono;
		std::lock_guard<std::recursive_mutex> lk(clients_cs);
        auto now = system_clock::now();
        if ( duration_cast<milliseconds>(now - last_ping_).count() > 5000) {
            std::cout << "stopping " << username_ << " - no ping in time" << std::endl;
            stop();
        }
		last_ping_ = system_clock::now();
    }

    void post_check_ping() {
		std::lock_guard<std::recursive_mutex> lk(clients_cs);
		timer_.expires_after(std::chrono::milliseconds(5000));
        timer_.async_wait( MEM_FN(on_check_ping));
    }


    void on_write(const error_code & err, size_t bytes) {
        do_read();
    }
    void do_read() {
        async_read(sock_, buffer(read_buffer_), 
                   MEM_FN2(read_complete,_1,_2), MEM_FN2(on_read,_1,_2));
        post_check_ping();
    }
    void do_write(const std::string & msg) {
        if ( !started() ) return;
		std::lock_guard<std::recursive_mutex> lk(clients_cs);
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
    mutable std::recursive_mutex cs_;
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
    std::string username_;
    system_timer timer_;
	std::chrono::system_clock::time_point last_ping_;
    bool clients_changed_;
};

void update_clients_changed() {
    array copy;
    { 
		std::lock_guard<std::recursive_mutex> lk(clients_cs);
		copy = clients;
    }
    for( array::iterator b = copy.begin(), e = copy.end(); b != e; ++b)
        (*b)->set_clients_changed();
}

ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));

void handle_accept(talk_to_client::ptr client, const std::error_code & err) {
    client->start();
    talk_to_client::ptr new_client = talk_to_client::new_();
    acceptor.async_accept(new_client->sock(), std::bind(handle_accept,new_client,_1));
}

std::vector<std::thread> threads;

void listen_thread() {
    service.run();
}

void start_listen(int thread_count) {
    for ( int i = 0; i < thread_count; ++i)
        threads.push_back( std::thread(listen_thread));
}

int main(int argc, char* argv[]) {
    talk_to_client::ptr client = talk_to_client::new_();
    acceptor.async_accept(client->sock(), std::bind(handle_accept,client,_1));
    start_listen(100);

	for (auto &thread: threads)
		thread.join();
}


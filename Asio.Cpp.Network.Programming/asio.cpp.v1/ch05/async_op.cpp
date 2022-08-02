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

struct async_op : std::enable_shared_from_this<async_op> {
    typedef std::shared_ptr<async_op> ptr;
    static ptr new_() { return ptr(new async_op); }

	async_op(const async_op &) = delete; 
	void operator =(const async_op &) = delete; 

private:
    typedef std::function<void(std::error_code)> completion_func;
    typedef std::function<std::error_code ()> op_func;

    async_op() : started_(false) {}

    struct operation {
        operation(io_context & service, op_func op, completion_func completion)
            : service(&service), op(op), completion(completion)
            , work(new io_context::work(service))
        {}
        operation() : service(0) {}
        io_context * service;
        op_func op;
        completion_func completion;
        typedef std::shared_ptr<io_context::work> work_ptr;
        work_ptr work;
    };

    typedef std::vector<operation> array;
    void start() {
		{ std::lock_guard<std::recursive_mutex> lk(cs_);
          if ( started_)
              return;
        started_ = true; }
        std::thread t( std::bind(&async_op::run,this));
		t.detach();
    }

    void run() {
		while ( true) {
			{ std::lock_guard<std::recursive_mutex> lk(cs_);
				if ( !started_) break; 
			}
			std::this_thread::sleep_for( std::chrono::milliseconds(10));
			operation cur;
			{ std::lock_guard<std::recursive_mutex> lk(cs_);
				if ( !ops_.empty()) {
					cur = ops_[0];
					ops_.erase( ops_.begin());
				}
			}
			if ( cur.service) {
				std::error_code err = cur.op();
				cur.service->post(std::bind(cur.completion, err));
			}
		}
        self_.reset();
    }

public:
    void add(op_func op, completion_func completion, io_context & service) {
        // so that we're not destroyed while async-executing something
        self_ = shared_from_this();
        std::lock_guard<std::recursive_mutex> lk(cs_);
        ops_.push_back( operation(service, op, completion));
        if ( !started_) start();
    }

    void stop() {
        std::lock_guard<std::recursive_mutex> lk(cs_);
        started_ = false;
        ops_.clear();
    }

private:
    array ops_;
    bool started_;
    std::recursive_mutex cs_;
    ptr self_;
};

size_t checksum = 0;
#ifdef WIN32
std::error_code compute_file_checksum(std::string file_name) {
    HANDLE file = ::CreateFile(file_name.c_str(), GENERIC_READ, 0, 0, 
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
    windows::random_access_handle h(service, file);
    long buff[1024];
    checksum = 0;
    size_t bytes = 0;
    size_t at = 0;
    std::error_code ec;
    while ( (bytes = read_at(h, at, buffer(buff), ec)) > 0) {
        at += bytes;
        bytes /= sizeof(long);
        for ( size_t i = 0; i < bytes; ++i)
            checksum += buff[i];
    }
    return std::error_code(0, std::generic_category());
}
#else
std::error_code compute_file_checksum(std::string file_name) {
    std::ifstream file(file_name);
    long buff[1024];
    checksum = 0;
    size_t bytes = 0;
    while ( (bytes = file.readsome((char *) buff, sizeof(buff))) > 0) {
        bytes /= sizeof(long);
        for ( size_t i = 0; i < bytes; ++i)
            checksum += buff[i];
    }
    return std::error_code(0, std::generic_category());
}
#endif

void on_checksum(std::string file_name, std::error_code) {
    std::cout << "checksum for " << file_name << "=" << checksum << std::endl;
}

int main(int argc, char* argv[]) {
    std::string fn = "readme.txt";
    async_op::new_()->add( std::bind(compute_file_checksum,fn),
                           std::bind(on_checksum,fn,_1), service);
    service.run();
}


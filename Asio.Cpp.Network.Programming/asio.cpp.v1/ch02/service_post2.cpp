#include <functional>
#include <iostream>
#include <vector>
#include <thread>
#include "asio.hpp"

asio::io_context service;

void func(int i) {
	std::cout << "func called, i= " << i << "/"
		<< std::this_thread::get_id() << std::endl;
}

void worker_thread() {
	service.run();
}

int main(int argc, char* argv[])
{
	asio::io_context::strand strand_one(service), strand_two(service);
	for ( int i = 0; i < 5; ++i)
		service.post( strand_one.wrap( std::bind(func, i)));
	for ( int i = 5; i < 10; ++i)
		service.post( strand_two.wrap( std::bind(func, i)));

	std::vector<std::thread> threads;
	for ( int i = 0; i < 3; ++i)
		threads.push_back(std::thread{worker_thread});

	// wait for all threads to be created
	std::this_thread::sleep_for( std::chrono::milliseconds(500));

	for (auto &thread: threads)
		thread.join();
}

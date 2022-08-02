#include <functional>
#include <iostream>
#include "asio.hpp"

asio::io_context service;

void func(int i) {
	std::cout << "func called, i= " << i << std::endl;
}

void run_dispatch_and_post() {
	for ( int i = 0; i < 10; i += 2) {
		service.dispatch(std::bind(func, i));
		service.post(std::bind(func, i + 1));
	}
}

int main(int argc, char* argv[]) {
	service.post(run_dispatch_and_post);
	service.run();
}

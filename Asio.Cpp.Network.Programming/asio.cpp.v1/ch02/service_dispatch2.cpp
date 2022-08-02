#include <thread>
#include <iostream>
#include "asio.hpp"

asio::io_context service;

void dispatched_func_1() {
	std::cout << "dispatched 1" << std::endl;
}

void dispatched_func_2() {
	std::cout << "dispatched 2" << std::endl;
}

void test(std::function<void()> func) {
	std::cout << "test" << std::endl;
	service.dispatch(dispatched_func_1);
	func();
}

void service_run() {
	service.run();
}

int main(int argc, char* argv[]) {
	test( service.wrap(dispatched_func_2));
	std::thread th(service_run);
	std::this_thread::sleep_for( std::chrono::milliseconds(500));
	th.join();
	return 0;
}

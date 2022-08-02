#include <iostream>
#include <system_error>

#include "asio.hpp"

int main()
{
	// Step 1. An instance of 'io_context' class is required by
	// socket constructor. 
	asio::io_context ioc;

	// Step 2. Creating an object of 'tcp' class representing
	// a TCP protocol with IPv4 as underlying protocol.
	asio::ip::tcp protocol = asio::ip::tcp::v4();

	try {
		// Step 3 + 4 in single call. May throw.
		asio::ip::tcp::socket sock(ioc, protocol);
	}
	catch (std::system_error & e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();
	}

	return 0;
}

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

	// Step 3. Instantiating an active TCP socket object.
	asio::ip::tcp::socket sock(ioc);

	// Used to store information about error that happens
	// while opening the socket.
	std::error_code ec;

	// Step 4. Opening the socket.
	sock.open(protocol, ec);

	if (ec.value() != 0) {
		// Failed to open the socket.
		std::cout
			<< "Failed to open the socket! Error code = "
			<< ec.value() << ". Message: " << ec.message();
		return ec.value();
	}

	return 0;
}

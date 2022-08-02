#include <iostream>
#include <system_error>
#include <string>

#include "asio.hpp"

int main()
{
	// Step 1. Assume that the client application has already
	// obtained the IP address and protocol port number of the
	// target server.
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;

	try {
		// Step 2. Creating an endpoint designating 
		// a target server application.
		asio::ip::tcp::endpoint
			ep(asio::ip::address::from_string(raw_ip_address),
			port_num);

		asio::io_context ioc;

		// Step 3. Creating and opening a socket.
		asio::ip::tcp::socket sock(ioc, ep.protocol());

		// Step 4. Connecting a socket.
		sock.connect(ep);
	}
	// Overloads of asio::ip::address::from_string() and 
	// asio::ip::tcp::socket::connect() used here throw
	// exceptions in case of error condition.
	catch (std::system_error &e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
}

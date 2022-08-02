#include <iostream>
#include <system_error>
#include <string>

#include "asio.hpp"

int main()
{
	// Step1. Assume that the client application has already
	// obtained the DNS name and protocol port number and
	// represented them as strings.
	std::string host = "samplehost.book";
	std::string port_num = "3333";

	// Used by a 'resolver' and a 'socket'.
	asio::io_context ioc;

	// Creating a resolver's query.
	asio::ip::tcp::resolver::query resolver_query(host, port_num,
		asio::ip::tcp::resolver::query::numeric_service);

	// Creating a resolver.
	asio::ip::tcp::resolver resolver(ioc);

	try {
		// Step 2. Resolving a DNS name.
		asio::ip::tcp::resolver::iterator it =
			resolver.resolve(resolver_query);

		// Step 3. Creating a socket.
		asio::ip::tcp::socket sock(ioc);

		// Step 4. asio::connect() method iterates over
		// each endpoint until successfully connects to one
		// of them. It will throw an exception if it fails
		// to connect to all the endpoints or if other
		// error occurs.
		asio::connect(sock, it);
	}
	// Overloads of asio::ip::tcp::resolver::resolve and 
	// asio::connect() used here throw
	// exceptions in case of error condition.
	catch (std::system_error &e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
}


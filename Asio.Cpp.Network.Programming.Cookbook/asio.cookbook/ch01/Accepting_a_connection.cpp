#include <iostream>
#include <system_error>

#include "asio.hpp"

int main()
{
	// The size of the queue containing the pending connection
	// requests.
	const int BACKLOG_SIZE = 30;

	// Step 1. Here we assume that the server application has
	// already obtained the protocol port number.
	unsigned short port_num = 3333;

	// Step 2. Creating a server endpoint.
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(),
		port_num);

	asio::io_context ioc;

	try {
		// Step 3. Instantiating and opening an acceptor socket.
		asio::ip::tcp::acceptor acceptor(ioc, ep.protocol());

		// Step 4. Binding the acceptor socket to the 
		// server endpint.
		acceptor.bind(ep);

		// Step 5. Starting to listen to incoming connection
		// requests.
		acceptor.listen(BACKLOG_SIZE);

		// Step 6. Creating an active socket.
		asio::ip::tcp::socket sock(ioc);

		// Step 7. Processing the next connection request and 
		// connecting the active socket to the client.
		acceptor.accept(sock);

		// At this point 'sock' socket is connected to 
		//the client application and can be used to send data to
		// or receive data from it.
	}
	catch (std::system_error &e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
}



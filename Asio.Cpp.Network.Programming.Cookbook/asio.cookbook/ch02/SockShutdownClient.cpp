#include <iostream>
#include <system_error>
#include <string>
#include <memory>

#include "asio.hpp"

void communicate(asio::ip::tcp::socket& sock) {
	// Allocating and filling the buffer with
	// binary data.
	const char request_buf[] = { 0x48, 0x65, 0x0, 0x6c, 0x6c, 0x6f };

	// Sending the request data.
	asio::write(sock, asio::buffer(request_buf));

	// Shutting down the socket to let the
	// server know that we've sent the whole
	// request.
	sock.shutdown(asio::socket_base::shutdown_send);

	// We use extensible buffer for response
	// because we don't know the size of the
	// response message.
	asio::streambuf response_buf;

	std::error_code ec;
	asio::read(sock, response_buf, ec);

	if (ec == asio::error::eof) {
		// Whole response message has been received.
		// Here we can handle it.
	}
	else {
		throw std::system_error(ec);
	}
}

int main()
{
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;

	try {
		asio::ip::tcp::endpoint
			ep(asio::ip::address::from_string(raw_ip_address),
			port_num);

		asio::io_context ioc;

		asio::ip::tcp::socket sock(ioc, ep.protocol());

		sock.connect(ep);

		communicate(sock);
	}
	catch (std::system_error &e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
}

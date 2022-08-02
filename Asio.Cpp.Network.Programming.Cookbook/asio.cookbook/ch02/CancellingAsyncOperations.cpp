// We need this to enable cancelling of I/O operations on
// Windows XP, Windows Server 2003 and earlier.
// Refer to "http://www.boost.org/doc/libs/1_58_0/
// doc/html/boost_asio/reference/basic_stream_socket/
// cancel/overload1.html" for details.
#ifdef WIN32
#define _WIN32_WINNT 0x0501
#endif

#include <iostream>
#include <thread>
#include <chrono>
#include <system_error>
#include <string>
#include <memory>

#include "asio.hpp"

int main()
{
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;

	try {
		asio::ip::tcp::endpoint
			ep(asio::ip::address::from_string(raw_ip_address),
			port_num);

		asio::io_context ioc;

		std::shared_ptr<asio::ip::tcp::socket> sock(
			new asio::ip::tcp::socket(ioc, ep.protocol()));

		sock->async_connect(ep,
			[sock](const std::error_code& ec)
		{
			// If asynchronous operation has been
			// cancelled or an error occured during
			// executon, ec contains corresponding
			// error code.
			if (ec) {
				if (ec == asio::error::operation_aborted) {
					std::cout << "Operation cancelled!";
				}
				else {
					std::cout << "Error occured!"
						<< " Error code = "
						<< ec.value()
						<< ". Message: "
						<< ec.message();
				}

				return;
			}
			// At this point the socket is connected and
			// can be used for communication with 
			// remote application.
		});

		// Starting a thread, which will be used
		// to call the callback when asynchronous 
		// operation completes.
		std::thread worker_thread([&ioc](){
			try {
				ioc.run();
			}
			catch (std::system_error &e) {
				std::cout << "Error occured!"
					<< " Error code = " << e.code()
					<< ". Message: " << e.what();
			}
		});

		// Emulating delay.
		std::this_thread::sleep_for(std::chrono::seconds(2));

		// Cancelling the initiated operation.
		sock->cancel();

		// Waiting for the worker thread to complete.
		worker_thread.join();
	}
	catch (std::system_error &e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
}

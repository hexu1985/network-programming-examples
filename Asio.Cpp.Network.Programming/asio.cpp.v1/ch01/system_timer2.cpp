#include <chrono>
#include <iostream>

#include "asio.hpp"

void handle_timeout(const asio::error_code&)
{
  std::cout << "handle_timeout\n";
}

int main()
{
  try
  {
    asio::io_context io_context;

	asio::system_timer timer(io_context);

    timer.expires_after(std::chrono::seconds(5));
    std::cout << "Starting synchronous wait\n";
    timer.wait();
    std::cout << "Finished synchronous wait\n";

    timer.expires_after(std::chrono::seconds(5));
    std::cout << "Starting asynchronous wait\n";
    timer.async_wait(&handle_timeout);
    io_context.run();
    std::cout << "Finished asynchronous wait\n";
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }

  return 0;
}

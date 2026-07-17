#include <iostream>
#include <iomanip>
#include <boost/asio.hpp>

void convert_integer() {
    uint32_t data = 1234;
    
    // 使用Boost.Asio的字节序转换函数
    uint32_t host_long = boost::asio::detail::socket_ops::network_to_host_long(data);
    uint32_t network_long = boost::asio::detail::socket_ops::host_to_network_long(data);
    
    std::cout << "Original: " << data 
              << " => Long host byte order: " << host_long 
              << ", Network byte order: " << network_long << std::endl;
    
    // 16-bit
    uint16_t data_short = 1234;
    uint16_t host_short = boost::asio::detail::socket_ops::network_to_host_short(data_short);
    uint16_t network_short = boost::asio::detail::socket_ops::host_to_network_short(data_short);
    
    std::cout << "Original: " << data_short 
              << " => Short host byte order: " << host_short 
              << ", Network byte order: " << network_short << std::endl;
}

int main() {
    convert_integer();
    return 0;
}

#include <iostream>
#include <string>
#include <asio.hpp>

void print_machine_info() {
    try {
        asio::io_context io_context;
        
        // 获取主机名
        std::string host_name = asio::ip::host_name();
        
        // 获取IP地址
        asio::ip::tcp::resolver resolver(io_context);
        asio::ip::tcp::resolver::results_type endpoints = 
            resolver.resolve(host_name, "");
        
        // 获取第一个IPv4地址
        std::string ip_address;
        for (const auto& endpoint : endpoints) {
            if (endpoint.endpoint().address().is_v4()) {
                ip_address = endpoint.endpoint().address().to_string();
                break;
            }
        }
        
        // 如果没有IPv4，使用第一个可用地址
        if (ip_address.empty() && !endpoints.empty()) {
            ip_address = endpoints.begin()->endpoint().address().to_string();
        }
        
        std::cout << "Host name: " << host_name << std::endl;
        std::cout << "IP address: " << ip_address << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    print_machine_info();
    return 0;
}

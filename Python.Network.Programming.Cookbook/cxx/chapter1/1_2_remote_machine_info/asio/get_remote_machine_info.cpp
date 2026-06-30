#include <iostream>
#include <string>
#include <asio.hpp>

void get_remote_machine_info() {
    const std::string remote_host = "www.python.org";
    
    try {
        asio::io_context io_context;
        asio::ip::tcp::resolver resolver(io_context);
        
        // 解析主机名
        asio::ip::tcp::resolver::results_type endpoints = 
            resolver.resolve(remote_host, "");
        
        // 获取第一个IP地址
        for (const auto& endpoint : endpoints) {
            std::string ip_address = endpoint.endpoint().address().to_string();
            std::string ip_version = endpoint.endpoint().address().is_v4() ? "IPv4" : "IPv6";
            
            std::cout << "IP address of " << remote_host 
                      << " (" << ip_version << "): " << ip_address << std::endl;
            break; // 只取第一个
        }
        
    } catch (const std::exception& e) {
        std::cerr << remote_host << ": " << e.what() << std::endl;
    }
}

int main() {
    get_remote_machine_info();
    return 0;
}

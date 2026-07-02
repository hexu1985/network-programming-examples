#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <asio.hpp>

void find_service_name() {
    try {
        asio::io_context io_context;
        asio::ip::tcp::resolver resolver(io_context);
        
        // TCP端口80和25
        std::vector<std::pair<int, std::string>> services = {
            {80, "tcp"},
            {25, "tcp"},
            {53, "udp"}
        };
        
        for (const auto& [port, protocol] : services) {
            try {
                // 使用resolver获取服务信息
                asio::ip::tcp::resolver::results_type results;
                
                if (protocol == "tcp") {
                    results = resolver.resolve("", std::to_string(port));
                } else {
                    // UDP使用不同的resolver
                    asio::ip::udp::resolver udp_resolver(io_context);
                    auto udp_results = udp_resolver.resolve("", std::to_string(port));
                    
                    // 提取服务名（如果有）
                    std::string service_name = "unknown";
                    for (const auto& result : udp_results) {
                        if (!result.service_name().empty()) {
                            service_name = result.service_name();
                            break;
                        }
                    }
                    std::cout << "Port: " << port 
                              << " => service name: " << service_name << std::endl;
                    continue;
                }
                
                // 提取TCP服务名
                std::string service_name = "unknown";
                for (const auto& result : results) {
                    if (!result.service_name().empty()) {
                        service_name = result.service_name();
                        break;
                    }
                }
                std::cout << "Port: " << port 
                          << " => service name: " << service_name << std::endl;
                
            } catch (const std::exception& e) {
                std::cout << "Port: " << port 
                          << " => service name: unknown (error: " << e.what() << ")" << std::endl;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    find_service_name();
    return 0;
}

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <boost/asio.hpp>

class ServiceLookup {
public:
    static std::string get_service_name(int port, const std::string& protocol) {
        try {
            boost::asio::io_context io_context;
            boost::asio::ip::tcp::resolver resolver(io_context);
            
            // 获取服务名称
            auto results = resolver.resolve(
                boost::asio::ip::tcp::v4(), 
                "", 
                std::to_string(port)
            );
            
            // 从结果中提取服务名称
            for (const auto& result : results) {
                if (!result.service_name().empty()) {
                    return result.service_name();
                }
            }
            return "unknown";
            
        } catch (const std::exception&) {
            return "unknown";
        }
    }
    
    static void print_service_info(int port, const std::string& protocol) {
        std::string service_name = get_service_name(port, protocol);
        std::cout << "Port: " << port 
                  << " => service name: " << service_name << std::endl;
    }
};

void find_service_name() {
    // 使用传统的getservbyport方式获取服务名
    // 注意：Boost.Asio的resolver主要用于域名解析，不直接提供服务名查找
    // 这里我们仍然使用POSIX API，但用C++风格封装
    
    auto get_service = [](int port, const std::string& protocol) -> std::string {
        struct servent* service = getservbyport(htons(port), protocol.c_str());
        return service ? std::string(service->s_name) : "unknown";
    };
    
    // TCP端口
    std::cout << "Port: " << 80 
              << " => service name: " << get_service(80, "tcp") << std::endl;
    std::cout << "Port: " << 25 
              << " => service name: " << get_service(25, "tcp") << std::endl;
    
    // UDP端口
    std::cout << "Port: " << 53 
              << " => service name: " << get_service(53, "udp") << std::endl;
}

int main() {
    find_service_name();
    return 0;
}

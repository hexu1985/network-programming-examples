#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>

class ServiceLookup {
public:
    static std::string get_service_name(int port, const std::string& protocol) {
        struct servent* service = getservbyport(htons(port), protocol.c_str());
        if (service != nullptr) {
            return std::string(service->s_name);
        }
        return "unknown";
    }
    
    static void print_service_info(int port, const std::string& protocol) {
        std::string service_name = get_service_name(port, protocol);
        std::cout << "Port: " << port 
                  << " => service name: " << service_name << std::endl;
    }
};

void find_service_name() {
    // TCP端口
    std::vector<std::pair<int, std::string>> services = {
        {80, "tcp"},
        {25, "tcp"},
        {53, "udp"}
    };
    
    for (const auto& [port, protocol] : services) {
        ServiceLookup::print_service_info(port, protocol);
    }
}

int main() {
    find_service_name();
    return 0;
}

#include <iostream>
#include <string>
#include <netdb.h>
#include <cstring>

void find_service_name() {
    const char* protocolname = "tcp";
    int ports[] = {80, 25};
    
    for (int port : ports) {
        struct servent* service = getservbyport(htons(port), protocolname);
        if (service != nullptr) {
            std::cout << "Port: " << port 
                      << " => service name: " << service->s_name << std::endl;
        } else {
            std::cerr << "Port: " << port 
                      << " => service name: unknown" << std::endl;
        }
    }
    
    // UDP端口53
    struct servent* service = getservbyport(htons(53), "udp");
    if (service != nullptr) {
        std::cout << "Port: " << 53 
                  << " => service name: " << service->s_name << std::endl;
    } else {
        std::cerr << "Port: " << 53 
                  << " => service name: unknown" << std::endl;
    }
}

int main() {
    find_service_name();
    return 0;
}

#include <iostream>
#include <iomanip>
#include <arpa/inet.h>

void convert_integer() {
    uint32_t data = 1234;
    
    // 32-bit转换
    uint32_t host_long = ntohl(data);
    uint32_t network_long = htonl(data);
    
    std::cout << "Original: " << data 
              << " => Long host byte order: " << host_long 
              << ", Network byte order: " << network_long << std::endl;
    
    // 16-bit转换
    uint16_t data_short = 1234;
    uint16_t host_short = ntohs(data_short);
    uint16_t network_short = htons(data_short);
    
    std::cout << "Original: " << data_short 
              << " => Short host byte order: " << host_short 
              << ", Network byte order: " << network_short << std::endl;
}

int main() {
    convert_integer();
    return 0;
}

#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iomanip>

void convert_ip4_address() {
    const char* ip_addresses[] = {"127.0.0.1", "192.168.0.1"};
    
    for (const char* ip_addr : ip_addresses) {
        struct in_addr addr;
        
        // 将点分十进制IP转换为二进制格式（网络字节序）
        if (inet_pton(AF_INET, ip_addr, &addr) != 1) {
            std::cerr << "Invalid IP address: " << ip_addr << std::endl;
            continue;
        }
        
        // 获取打包的二进制数据
        unsigned char* packed = reinterpret_cast<unsigned char*>(&addr);
        
        // 将二进制IP转换回点分十进制
        char unpacked[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &addr, unpacked, sizeof(unpacked)) == nullptr) {
            std::cerr << "Error converting IP: " << ip_addr << std::endl;
            continue;
        }
        
        // 输出结果
        std::cout << "IP Address: " << ip_addr << " => Packed: ";
        
        // 以十六进制格式输出打包的IP（网络字节序）
        for (size_t i = 0; i < sizeof(addr); ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') 
                      << static_cast<int>(packed[i]);
        }
        std::cout << std::dec << ", Unpacked: " << unpacked << std::endl;
    }
}

int main() {
    convert_ip4_address();
    return 0;
}

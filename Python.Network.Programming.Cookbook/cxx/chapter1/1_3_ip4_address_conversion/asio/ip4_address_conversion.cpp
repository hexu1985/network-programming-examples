#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <asio.hpp>

void convert_ip4_address() {
    std::vector<std::string> ip_addresses = {"127.0.0.1", "192.168.0.1"};
    
    for (const auto& ip_addr : ip_addresses) {
        try {
            // 将字符串IP转换为地址对象
            asio::ip::address_v4 addr = asio::ip::make_address_v4(ip_addr);
            
            // 获取打包的二进制数据（网络字节序）
            asio::ip::address_v4::bytes_type packed_bytes = addr.to_bytes();
            
            // 将二进制转换回字符串
            std::string unpacked = addr.to_string();
            
            // 输出结果
            std::cout << "IP Address: " << ip_addr << " => Packed: ";
            
            // 以十六进制格式输出打包的IP
            for (unsigned char byte : packed_bytes) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') 
                          << static_cast<int>(byte);
            }
            std::cout << std::dec << ", Unpacked: " << unpacked << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "Error processing IP " << ip_addr << ": " << e.what() << std::endl;
        }
    }
}

int main() {
    convert_ip4_address();
    return 0;
}

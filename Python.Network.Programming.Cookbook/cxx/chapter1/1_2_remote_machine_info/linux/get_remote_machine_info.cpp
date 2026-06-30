#include <iostream>
#include <string>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>

void get_remote_machine_info() {
    const std::string remote_host = "www.python.org";
    
    // 解析主机名
    struct hostent *host_entry = gethostbyname(remote_host.c_str());
    
    if (host_entry == nullptr) {
        // 处理错误
        std::cerr << remote_host << ": " << hstrerror(h_errno) << std::endl;
        return;
    }
    
    // 获取第一个IP地址（IPv4）
    struct in_addr *addr = (struct in_addr*)host_entry->h_addr_list[0];
    char ip_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, addr, ip_address, sizeof(ip_address));
    
    std::cout << "IP address of " << remote_host << ": " << ip_address << std::endl;
}

int main() {
    get_remote_machine_info();
    return 0;
}

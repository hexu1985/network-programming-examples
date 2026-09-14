// list_interfaces.cpp
// 等价于 Python Network Programming Cookbook 中的 list_interfaces
// 编译: g++ -std=c++11 -o list_interfaces list_interfaces.cpp
// 运行: ./list_interfaces

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

// 与 Python 代码保持一致
const unsigned long SIOCGIFCONF_CONST = 0x8912; // from C library sockios.h
const int STUCT_SIZE_32 = 32;
const int STUCT_SIZE_64 = 40;
const unsigned long long PLATFORM_32_MAX_NUMBER = 1ULL << 32;
const int DEFAULT_INTERFACES = 8;

std::vector<std::string> list_interfaces()
{
    std::vector<std::string> interfaces;

    int max_interfaces = DEFAULT_INTERFACES;

    // 判断是否为 64 位平台: 等价于 Python 的 sys.maxsize > 2**32
    bool is_64bits = sizeof(void *) > 4;
    int struct_size = is_64bits ? STUCT_SIZE_64 : STUCT_SIZE_32;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        return interfaces;
    }

    int outbytes = 0;
    std::vector<char> interface_names;

    while (true)
    {
        int bytes = max_interfaces * struct_size;
        interface_names.assign(bytes, '\0');

        // 构造 ifconf 结构, 等价于 struct.pack('iL', bytes, buf_addr)
        struct ifconf ifc;
        ifc.ifc_len = bytes;
        ifc.ifc_buf = interface_names.data();

        if (ioctl(sock, SIOCGIFCONF, &ifc) < 0)
        {
            perror("ioctl SIOCGIFCONF");
            close(sock);
            return interfaces;
        }

        outbytes = ifc.ifc_len;

        if (outbytes == bytes)
        {
            // 缓冲区可能不够, 翻倍重试
            max_interfaces *= 2;
        }
        else
        {
            break;
        }
    }

    // 逐个解析接口名, 与原 Python 代码逻辑一致:
    // 每个条目长度为 struct_size, 前 16 字节为接口名 (以 '\0' 结尾)
    for (int i = 0; i < outbytes; i += struct_size)
    {
        const char *name_ptr = interface_names.data() + i;
        // 取最多 16 字节, 遇到 '\0' 截断
        size_t len = strnlen(name_ptr, 16);
        interfaces.push_back(std::string(name_ptr, len));
    }

    close(sock);
    return interfaces;
}

int main()
{
    std::vector<std::string> interfaces = list_interfaces();

    std::cout << "This machine has " << interfaces.size()
              << " network interfaces: [";

    for (size_t i = 0; i < interfaces.size(); ++i)
    {
        if (i > 0)
            std::cout << ", ";
        std::cout << "'" << interfaces[i] << "'";
    }

    std::cout << "]." << std::endl;

    return 0;
}

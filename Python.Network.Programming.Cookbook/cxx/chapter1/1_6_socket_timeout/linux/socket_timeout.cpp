#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

inline double timeval_to_double(const struct timeval& tv) {
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}

void test_socket_timeout() {
    // 创建socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return;
    }
    
    // 获取默认超时时间
    struct timeval tv;
    socklen_t len = sizeof(tv);
    
    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, &len) == 0) {
        std::cout << "Default socket timeout: "
                  << timeval_to_double(tv) << " seconds" << std::endl;
    } else {
        std::cout << "Default socket timeout: None (or error getting timeout)" << std::endl;
    }
    
    // 设置超时时间为100秒
    tv.tv_sec = 100;
    tv.tv_usec = 0;
    
    // 设置接收超时
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        std::cerr << "Failed to set receive timeout: " << strerror(errno) << std::endl;
    }
    
    // 设置发送超时
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0) {
        std::cerr << "Failed to set send timeout: " << strerror(errno) << std::endl;
    }
    
    // 获取新的超时时间
    len = sizeof(tv);
    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, &len) == 0) {
        std::cout << "Current socket timeout: " 
                  << timeval_to_double(tv) << " seconds" << std::endl;
    } else {
        std::cout << "Current socket timeout: None (or error getting timeout)" << std::endl;
    }
    
    // 关闭socket
    close(sockfd);
}

int main() {
    test_socket_timeout();
    return 0;
}

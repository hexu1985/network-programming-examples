#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <getopt.h>

struct Arguments {
    std::string host;
    int port = 0;
    std::string filename;
};

Arguments parse_arguments(int argc, char* argv[]) {
    Arguments args;
    int opt;
    
    // 长选项定义
    static struct option long_options[] = {
        {"host", required_argument, 0, 'h'},
        {"port", required_argument, 0, 'p'},
        {"file", required_argument, 0, 'f'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "h:p:f:", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'h':
                args.host = optarg;
                break;
            case 'p':
                args.port = std::stoi(optarg);
                break;
            case 'f':
                args.filename = optarg;
                break;
            default:
                std::cerr << "Usage: " << argv[0] 
                          << " --host <host> --port <port> --file <filename>" << std::endl;
                exit(EXIT_FAILURE);
        }
    }
    
    if (args.host.empty() || args.port == 0 || args.filename.empty()) {
        std::cerr << "Error: host, port, and file are required" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    return args;
}

int main(int argc, char* argv[]) {
    try {
        // 解析参数
        Arguments args = parse_arguments(argc, argv);
        
        // 创建socket
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
            return EXIT_FAILURE;
        }
        
        // 解析主机名
        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        
        int status = getaddrinfo(args.host.c_str(), std::to_string(args.port).c_str(), 
                                 &hints, &res);
        if (status != 0) {
            std::cerr << "Address-related error connecting to server: " 
                      << gai_strerror(status) << std::endl;
            close(sockfd);
            return EXIT_FAILURE;
        }
        
        // 连接服务器
        bool connected = false;
        for (struct addrinfo* p = res; p != nullptr; p = p->ai_next) {
            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == 0) {
                connected = true;
                break;
            }
        }
        
        freeaddrinfo(res);
        
        if (!connected) {
            std::cerr << "Connection error: " << strerror(errno) << std::endl;
            close(sockfd);
            return EXIT_FAILURE;
        }
        
        // 构造HTTP GET请求
        std::string request = "GET " + args.filename + " HTTP/1.0\r\n\r\n";
        
        // 发送数据
        ssize_t sent = send(sockfd, request.c_str(), request.length(), 0);
        if (sent < 0) {
            std::cerr << "Error sending data: " << strerror(errno) << std::endl;
            close(sockfd);
            return EXIT_FAILURE;
        }
        
        // 接收数据
        char buffer[2048];
        ssize_t bytes_received;
        while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[bytes_received] = '\0';
            std::cout << buffer;
        }
        
        if (bytes_received < 0) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
            close(sockfd);
            return EXIT_FAILURE;
        }
        
        close(sockfd);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

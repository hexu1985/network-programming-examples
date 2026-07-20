#include <iostream>
#include <string>
#include <asio.hpp>
#include <getopt.h>

struct Arguments {
    std::string host;
    int port = 0;
    std::string filename;
};

Arguments parse_arguments(int argc, char* argv[]) {
    Arguments args;
    int opt;
    
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
        
        asio::io_context io_context;
        
        // 创建socket
        asio::ip::tcp::socket socket(io_context);
        
        // 解析主机和端口
        asio::ip::tcp::resolver resolver(io_context);
        asio::ip::tcp::resolver::results_type endpoints = 
            resolver.resolve(args.host, std::to_string(args.port));
        
        // 连接服务器
        asio::connect(socket, endpoints);
        
        // 发送HTTP请求
        std::string request = "GET " + args.filename + " HTTP/1.0\r\n\r\n";
        asio::write(socket, asio::buffer(request));
        
        // 接收响应
        char buffer[2048];
        asio::error_code error;
        while (true) {
            size_t bytes = socket.read_some(asio::buffer(buffer), error);
            if (error == asio::error::eof) {
                break; // 连接正常关闭
            } else if (error) {
                throw asio::system_error(error);
            }
            std::cout.write(buffer, bytes);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

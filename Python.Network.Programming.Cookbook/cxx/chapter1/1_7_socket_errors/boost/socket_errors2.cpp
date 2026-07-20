#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "cxxopts.hpp"

struct Arguments {
    std::string host;
    int port = 0;
    std::string filename;
};

Arguments parse_arguments(int argc, char* argv[]) {
    Arguments args;
    try {
        cxxopts::Options options("Socket Error Examples", "Socket Error Examples");
        
        options.add_options()
            ("host", "Host address", cxxopts::value<std::string>())
            ("port", "Port number", cxxopts::value<int>())
            ("file", "File name", cxxopts::value<std::string>())
            ("help", "Print help");
        
        auto result = options.parse(argc, argv);
        
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(1);
        }
        
        // Get values with defaults if not specified
        args.host = result["host"].as<std::string>();
        args.port = result["port"].as<int>();
        args.filename = result["file"].as<std::string>();
        
        // Use the variables
        std::cout << "Host: " << args.host << std::endl;
        std::cout << "Port: " << args.port << std::endl;
        std::cout << "File: " << args.filename << std::endl;
        
    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        exit(1);
    }

    return args;
}

int main(int argc, char* argv[]) {
    try {
        // 解析参数
        Arguments args = parse_arguments(argc, argv);
        
        boost::asio::io_context io_context;
        
        // 创建socket
        boost::asio::ip::tcp::socket socket(io_context);
        
        // 解析主机和端口
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::results_type endpoints = 
            resolver.resolve(args.host, std::to_string(args.port));
        
        // 连接服务器
        boost::asio::connect(socket, endpoints);
        
        // 发送HTTP请求
        std::string request = "GET " + args.filename + " HTTP/1.0\r\n\r\n";
        boost::asio::write(socket, boost::asio::buffer(request));
        
        // 接收响应
        char buffer[2048];
        boost::system::error_code error;
        while (true) {
            size_t bytes = socket.read_some(boost::asio::buffer(buffer), error);
            if (error == boost::asio::error::eof) {
                break; // 连接正常关闭
            } else if (error) {
                throw boost::system::system_error(error);
            }
            std::cout.write(buffer, bytes);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

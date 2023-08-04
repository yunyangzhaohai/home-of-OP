#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 8888;
const std::string SERVER_IP = "127.0.0.1";

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[1024];

    // 创建客户端套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    // 设置服务器地址结构
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    // 将IP地址转换为网络字节序
    if (inet_pton(AF_INET, SERVER_IP.c_str(), &serverAddress.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        return -1;
    }

    // 连接服务器
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to connect to server" << std::endl;
        return -1;
    }

    std::cout << "Connected to server" << std::endl;

    // 发送和接收数据
    while (true) {
        std::string message;
        std::cout << "Enter a message: ";
        std::getline(std::cin, message);

        // 发送消息到服务器
        send(clientSocket, message.c_str(), message.length(), 0);

        // 接收服务器的响应消息
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Connection closed by server" << std::endl;
            break;
        }

        std::cout << "Server response: " << buffer << std::endl;
    }

    close(clientSocket);

    std::cout << "Client disconnected" << std::endl;

    return 0;
}

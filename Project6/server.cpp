#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 8888;

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[1024];

    // 创建服务器套接字
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    // 设置服务器地址结构
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // 绑定服务器套接字到指定地址和端口
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        return -1;
    }

    // 监听传入的连接请求
    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Failed to listen" << std::endl;
        return -1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    socklen_t clientAddressSize = sizeof(clientAddress);

    // 接受客户端连接
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if (clientSocket < 0) {
        std::cerr << "Failed to accept connection" << std::endl;
        return -1;
    }

    std::cout << "Client connected" << std::endl;

    // 接收和发送数据
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Connection closed by client" << std::endl;
            break;
        }

        std::cout << "Received: " << buffer << std::endl;

        // 对接收到的数据进行处理
        std::string response = "Server response";
        send(clientSocket, response.c_str(), response.length(), 0);
    }

    close(clientSocket);
    close(serverSocket);

    std::cout << "Server shut down" << std::endl;

    return 0;
}

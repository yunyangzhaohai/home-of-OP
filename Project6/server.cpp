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

    // �����������׽���
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    // ���÷�������ַ�ṹ
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // �󶨷������׽��ֵ�ָ����ַ�Ͷ˿�
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        return -1;
    }

    // �����������������
    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Failed to listen" << std::endl;
        return -1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    socklen_t clientAddressSize = sizeof(clientAddress);

    // ���ܿͻ�������
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if (clientSocket < 0) {
        std::cerr << "Failed to accept connection" << std::endl;
        return -1;
    }

    std::cout << "Client connected" << std::endl;

    // ���պͷ�������
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Connection closed by client" << std::endl;
            break;
        }

        std::cout << "Received: " << buffer << std::endl;

        // �Խ��յ������ݽ��д���
        std::string response = "Server response";
        send(clientSocket, response.c_str(), response.length(), 0);
    }

    close(clientSocket);
    close(serverSocket);

    std::cout << "Server shut down" << std::endl;

    return 0;
}

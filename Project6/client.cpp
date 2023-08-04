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

    // �����ͻ����׽���
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    // ���÷�������ַ�ṹ
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    // ��IP��ַת��Ϊ�����ֽ���
    if (inet_pton(AF_INET, SERVER_IP.c_str(), &serverAddress.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        return -1;
    }

    // ���ӷ�����
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to connect to server" << std::endl;
        return -1;
    }

    std::cout << "Connected to server" << std::endl;

    // ���ͺͽ�������
    while (true) {
        std::string message;
        std::cout << "Enter a message: ";
        std::getline(std::cin, message);

        // ������Ϣ��������
        send(clientSocket, message.c_str(), message.length(), 0);

        // ���շ���������Ӧ��Ϣ
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

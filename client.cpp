#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

#define PORT 8080

int main() {
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set to localhost

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server\n";

    while (true) {
        std::cout << "Enter message: ";
        std::string message;
        std::getline(std::cin, message);

        // Send message to server
        send(sockfd, message.c_str(), message.length(), 0);

        // Receive server response
        memset(buffer, 0, sizeof(buffer));
        int n = recv(sockfd, buffer, sizeof(buffer), 0);
        if (n <= 0) {
            std::cerr << "Server disconnected" << std::endl;
            break;
        }
        std::cout << "Server: " << buffer << std::endl;
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}

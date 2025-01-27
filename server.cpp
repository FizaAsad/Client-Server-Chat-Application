#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

#define PORT 8080

int main() {
    WSADATA wsaData;
    SOCKET sockfd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(client_addr);
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

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces

    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(sockfd, 1) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << "...\n";

    // Accept incoming connection
    new_sock = accept(sockfd, (struct sockaddr*)&client_addr, &addr_len);
    if (new_sock == INVALID_SOCKET) {
        std::cerr << "Accept failed" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    std::cout << "Connection established with client\n";

    // Communicate with the client
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int n = recv(new_sock, buffer, sizeof(buffer), 0);
        if (n <= 0) {
            std::cerr << "Client disconnected" << std::endl;
            break;
        }
        std::cout << "Client: " << buffer << std::endl;

        // Send response back to client
        std::string response = "Message received: " + std::string(buffer);
        send(new_sock, response.c_str(), response.length(), 0);
    }

    closesocket(new_sock);
    closesocket(sockfd);
    WSACleanup();
    return 0;
}

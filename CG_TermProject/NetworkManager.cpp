#include "NetworkManager.h"


NetworkManager::NetworkManager()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        exit(1);
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        WSACleanup();
        exit(1);
    }
}

NetworkManager::~NetworkManager() {
    closesocket(clientSocket);
    WSACleanup();
}

bool NetworkManager::Connect(const char* ipaddress) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, ipaddress, &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Failed to connect to the server" << std::endl;
        return false;
    }

    return true;
}


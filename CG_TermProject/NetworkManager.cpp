#include "NetworkManager.h"
#include "Scene.h"

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

bool NetworkManager::Connect() {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Failed to connect to the server" << std::endl;
        return false;
    }

    return true;
}

bool NetworkManager::RecvThread() {

    char buf[1024];
    // 패킷 전체 데이터 수신
    int bytesReceived = recv(clientSocket, buf, sizeof(buf), 0);

    if (bytesReceived <= 0) {
        // 연결 종료 또는 오류 처리
        if (bytesReceived == 0) {
            std::cout << "Client disconnected." << std::endl;
        }
        else {
            std::cout << "Recv error: " << WSAGetLastError() << std::endl;
        }
        return false;
    }
 
    char packetType = static_cast<char>(buf[1]);
    
    // 패킷 처리
    switch (packetType) {
    default:
        std::cout << "Unknown packet type: " << packetType << std::endl;
        break;
    }

    return true;
}

void NetworkManager::SetScene(std::shared_ptr<Scene> scene)
{
    m_Scene = scene;
    printf("qwqw");
}

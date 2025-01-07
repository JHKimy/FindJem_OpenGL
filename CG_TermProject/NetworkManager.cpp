#include "NetworkManager.h"
#include "Scene.h"
#include "Global.h"

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

bool NetworkManager::Connect() 
{
    // ���� �ּ� ���� ����ü
    sockaddr_in serverAddr;
    // IPv4 ���� 
    serverAddr.sin_family = AF_INET;
    // ���� ��Ʈ�� ��Ʈ��ũ ����Ʈ ������ ��ȯ �� ����
    serverAddr.sin_port = htons(SERVER_PORT);
    // ���� IP �ּҸ� ��Ʈ��ũ ����Ʈ ������ ��ȯ �� ����
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // ������ ���� �õ�
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Failed to connect to the server" << std::endl;
        return false;
    }
    // ���� ���� ó��
    return true;
}

bool NetworkManager::RecvCharacterInfo()
{
    char buf[1024];
    int bytesReceived = recv(clientSocket, buf, sizeof(buf), 0);

    if (bytesReceived <= 0) {
        if (bytesReceived == 0) {
            std::cout << "Server closed the connection." << std::endl;
        }
        else {
            std::cout << "Recv error: " << WSAGetLastError() << std::endl;
        }
        return false;
    }

    // 3. ��Ŷ Ÿ�� ó��
    char packetType = static_cast<char>(buf[1]);
    if (packetType == SC_MAZE_DATA) {
        SC_MY_CHARACTER* p = reinterpret_cast<SC_MY_CHARACTER*>(buf);
        // Ŭ���̾�Ʈ �� id ����(��Ŷ send�� id �ĺ� �뵵)
        startPos.x = p->x;
        startPos.y = p->y;
        startPos.z = p->z;
    }
    else {
        std::cout << "Unknown packet type: " << packetType << std::endl;
    }
    return false;
}

bool NetworkManager::RecvThread() {

    char buf[1024];
    // ��Ŷ ��ü ������ ����
    int bytesReceived = recv(clientSocket, buf, sizeof(buf), 0);

    if (bytesReceived <= 0) {
        // ���� ���� �Ǵ� ���� ó��
        if (bytesReceived == 0) {
            std::cout << "Client disconnected." << std::endl;
        }
        else {
            std::cout << "Recv error: " << WSAGetLastError() << std::endl;
        }
        return false;
    }
 
    char packetType = static_cast<char>(buf[1]);
    
    // ��Ŷ ó��
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

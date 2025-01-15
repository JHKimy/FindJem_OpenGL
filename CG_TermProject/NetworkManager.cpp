#include "NetworkManager.h"
#include "Scene.h"
#include "Global.h"
#include "Character.h"


NetworkManager::NetworkManager()
{
    // Winsock �ʱ�ȭ
    WSADATA wsaData;
    // Winsock 2.2���� �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        exit(1);
    }

    // Ŭ���̾�Ʈ ���� ����
    // (IPv4, TCP, �⺻ ��������)
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

// ������ ����
bool NetworkManager::Connect()
{
    // ���� �ּ� ���� ����ü
    sockaddr_in serverAddr;
    // IPv4 ���� 
    serverAddr.sin_family = AF_INET;
    // �������ݿ��� ���� ��Ʈ ������ ����
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

// ���� ������ �ޱ�
bool NetworkManager::RecvThread() {



    // ������ ���� ����
    char buf[BUFSIZE];

    // �����κ��� ������ ����
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

    // ��Ŷ�� 2��°�� ��Ŷ Ÿ��
    // ���������� ��ȯ
    char packetType = static_cast<char>(buf[1]);

    // ��Ŷ ó��
    switch (packetType) {

        // ĳ���� �̵� ��Ŷ
    case SC_ADD_CHARACTER:
    {
        SC_ADD_CHARACTER_PACKET* p = reinterpret_cast<SC_ADD_CHARACTER_PACKET*>(buf);
        position.x = p->PosX;
        position.y = p->PosY;
        position.z = p->PosZ;
        g_bReady = true;

        /*otherCharacter1 = make_unique<Character>(glm::vec3(position.x, position.y, position.z));
        otherCharacter1->Render(shaderProgram);*/
        break;
    }

    // ĳ���� �̵� ��Ŷ
    case SC_CHARACTER_MOVE:
    {
        SC_CHARACTER_MOVE_PACKET* p = reinterpret_cast<SC_CHARACTER_MOVE_PACKET*>(buf);
        m_Scene->GetCharacter()->SetPosition(glm::vec3(p->PosX, p->PosY, p->PosZ));
        m_Scene->GetCharacter()->SetForwardVector(glm::vec3(p->DirX, 0.f, p->DirZ));
        m_Scene->GetCharacter()->SetYaw(p->yaw);
        //cout <<p->yaw<< endl;
        break;
    }
    case SC_OTHER_CHARACTER_MOVE:
    {
        if (g_isOtherCharacter) {
            SC_0THER_CHARACTER_MOVE_PACKET* p = reinterpret_cast<SC_0THER_CHARACTER_MOVE_PACKET*>(buf);
            // �ٸ� �÷��̾� ��ġ, fv, yaw ó��
            m_Scene->GetOtherCharacter1()->SetPosition(glm::vec3(p->PosX, p->PosY, p->PosZ));
            m_Scene->GetOtherCharacter1()->SetForwardVector(glm::vec3(p->DirX, 0.f, p->DirZ));
            m_Scene->GetOtherCharacter1()->SetYaw(p->yaw);
            break;
        }
        break;
    }

    default:
        std::cout << "Unknown packet type: " << packetType << std::endl;
        break;
    }
    return true;
}

void NetworkManager::SendReady()
{
    CS_READY_PACKET p;
    p.packet_size = sizeof(p);
    p.packet_type = CS_READY;
    p.player_id = g_id;

    int retval = send(clientSocket,
        reinterpret_cast<const char*>(&p), sizeof(p), 0);
}

void NetworkManager::SendPlayerMove(CS_PLAYER_PACKET& p)
{
    p.packet_size = sizeof(p);
    p.packet_type = CS_PLAYER;
    p.player_id = g_id;

    int retval = send(clientSocket,
        reinterpret_cast<const char*>(&p), sizeof(p), 0);

}

void NetworkManager::SetScene(std::shared_ptr<Scene> scene)
{
    m_Scene = scene;
}
void NetworkManager::RecvMazeData()
{
    char buf[BUFSIZE];
    int bytesReceived = recv(clientSocket, buf, sizeof(buf), 0);

    if (bytesReceived <= 0) {
        if (bytesReceived == 0) {
            std::cout << "Server closed the connection." << std::endl;
        }
        else {
            std::cout << "Recv error: " << WSAGetLastError() << std::endl;
        }
        return;
    }

    // 3. ��Ŷ Ÿ�� ó��
    char packetType = static_cast<char>(buf[1]);
    if (packetType == SC_MAZE_DATA) {
        SC_MAZE_INFO* p = reinterpret_cast<SC_MAZE_INFO*>(buf);
        // Ŭ���̾�Ʈ �� id ����(��Ŷ send�� id �ĺ� �뵵)
        g_id = p->player_id;
        startPos.x = p->x;
        startPos.y = p->y;
        startPos.z = p->z;
        // 4. ���� �̷� ������ ����
        m_Scene->SetMaze(p->mazeMap);

    }


    else {
        std::cout << "Unknown packet type: " << packetType << std::endl;
    }
}

void NetworkManager::RecvEnemiesData()
{
    char buf[BUFSIZE];
    int bytesReceived = recv(clientSocket, buf, sizeof(SC_ENEMY_PACKET), 0);

    if (bytesReceived <= 0) {
        if (bytesReceived == 0) {
            std::cout << "Server closed the connection." << std::endl;
        }
        else {
            std::cout << "Recv error: " << WSAGetLastError() << std::endl;
        }
        return;
    }

    if (bytesReceived != sizeof(SC_ENEMY_PACKET)) {
        std::cout << "Incomplete packet received. Expected: " << sizeof(SC_ENEMY_PACKET)
            << ", Received: " << bytesReceived << std::endl;
        return;
    }

    SC_ENEMY_PACKET* p = reinterpret_cast<SC_ENEMY_PACKET*>(buf);

    if (p->packet_type == SC_ENEMY) {
        std::cout << "Enemy packet received. EnemyID: " << p->enemy_id
            << " Position: (" << p->PosX << ", " << p->PosY << ", " << p->PosZ << ")"
            << std::endl;

        // �� ��ü ���� ����
        //m_Scene->enemies[p->enemy_id] = make_shared<Enemy>(glm::vec3(p->PosX, p->PosY, p->PosZ));
    }
    else {
        std::cout << "Unknown packet type: " << (int)p->packet_type << std::endl;
    }
}

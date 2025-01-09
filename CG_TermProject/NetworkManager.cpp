#include "NetworkManager.h"
#include "Scene.h"
#include "Global.h"
#include "Character.h"


NetworkManager::NetworkManager()
{   
    // Winsock 초기화
    WSADATA wsaData;
    // Winsock 2.2버전 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        exit(1);
    }

    // 클라이언트 소켓 생성
    // (IPv4, TCP, 기본 프로토콜)
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

// 서버에 연결
bool NetworkManager::Connect() 
{
    // 서버 주소 정보 구조체
    sockaddr_in serverAddr;
    // IPv4 설정 
    serverAddr.sin_family = AF_INET;
    // 프로토콜에서 서버 포트 가져와 설정
    serverAddr.sin_port = htons(SERVER_PORT);
    // 서버 IP 주소를 네트워크 바이트 순서로 변환 후 설정
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // 서버에 연결 시도
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Failed to connect to the server" << std::endl;
        return false;
    }
    // 연결 성공 처리
    return true;
}

// 서버 데이터 받기
bool NetworkManager::RecvThread() {

    // 데이터 저장 버퍼
    char buf[1024];

    // 서버로부터 데이터 수신
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
 
    // 패킷의 2번째로 패킷 타입
    // 문자형으로 반환
    char packetType = static_cast<char>(buf[1]);
    
    // 패킷 처리
    switch (packetType) {

    // 캐릭터 이동 패킷
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

    // 캐릭터 이동 패킷
    case SC_CHARACTER_MOVE: 
    {
        SC_CHARACTER_MOVE_PACKET* p = reinterpret_cast<SC_CHARACTER_MOVE_PACKET*>(buf);
        m_Scene->GetCharacter()->SetPosition(glm::vec3(p->PosX, p->PosY, p->PosZ));
        m_Scene->GetCharacter()->SetForwardVector(glm::vec3(p->DirX, 0.f, p->DirZ));
        m_Scene->GetCharacter()->SetYaw(p->yaw);
        break;
    }
    case SC_OTHER_CHARACTER_MOVE: {
        SC_0THER_CHARACTER_MOVE_PACKET* p = reinterpret_cast<SC_0THER_CHARACTER_MOVE_PACKET*>(buf);
        // 다른 플레이어 위치, fv, yaw 처리
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
